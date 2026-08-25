**********************************************************************************
*  ____    _   _    ____        ____    _____   ____     ___    ____    _____    *
* | __ )  | | | |  / ___|       |  _ \  | ____| |  _ \   / _ \  |  _ \  |_   _|  *
* |  _ \  | | | | | |  _        | |_) | |  _|   | |_) | | | | | | |_) |   | |    *
* | |_) | | |_| | | |_| |       |  _ <  | |___  |  __/  | |_| | |  _ <    | |    *
* |____/   \___/   \____|       |_| \_\ |_____| |_|      \___/  |_| \_\   |_|    *
*                                                                                *
**********************************************************************************

1. Summary

The USB enumeration failure in the bootloader was traced to the ordering of operations
in the USB RESET interrupt handler. When the host connected to the device, the Blue
Pill correctly detected the USB bus reset and generated the USB interrupt. The CPU 
consequently entered USB_LP_CAN_RX0_IRQHandler, and inspection of USB->ISTR confirmed
that the RESET event was active. The handler however performed reset_endpoints()
before clearing the ISTR.RESET event:

if( istr & USB_ISTR_bits::RESET ) {
    reset_endpoints();
    USB->ISTR = ~(USB_ISTR_bits::RESET );
    return;
}

The required ordering is:

if( istr & USB_ISTR_bits::RESET ) {
    USB->ISTR = ~(USB_ISTR_bits::RESET );
    reset_endpoints();
    return;
}

The problem was therefore not an inability of the USB interrpt to fire, not a failure
of NVIC, not an incorrect interrupt-vector entry, and not primarily a PMA/BTABLE addressing
proble. The debugging showed that the USB RESET event was reching the ISR, the PMA buffer
-table fields were being written, and DADDR was accepting its configuration, while EP0's
endpoint register did not exhibit the expected configuration when endpoint initialization 
was performed whiel the RESET event was still active. The root cause was consequently 
isolated to the sequencing of USB reset-event handling and endpoint reinitialization.

The correction required moving the ISTR.RESET clear before reset_endpoints(), but the 
debugging process was important because the initial symptoms appeared much farther downstream:
USB enumeration did not complete and the firmware's LED continued blinking instead of reaching
the expected solid state.


────────────────────────────────────────────────────────────────────────────────────────────────────


2. Expected USB Initialization and Enumeration Sequence

The expected sequence after the host connects to the STM32F103 USB device is as follows 
sequentially:

-> USB host detects device

-> HOST issues USB bus reset

-> STM32 USB Peripheral detects RESET

-> ISTR.RESET becomes asserted

-> USB interrupt is generated

-> USB_LP_CAN_RX0_IRQHandler() is entered

-> RESET event is cleared

-> reset_endpoints() is called

    --> Endpoint/PMA state is initialized

    --> EP0 configured as a control endpoint

    --> EP0 RX made VALID
    
    --> USB device becomes ready for  control traffic

-> Host sends SETUP packet

-> USB SIE receives SETUP

-> EP0 CTR_RX and SETUP status becomes asserted

-> ISTR.CTR becomes asserted

-> USB ISR processes the contorl transfer

-> USB enumeration proceeds


The RESET interrupt is therefore a transition point between the USB bus reset state
and the normal USB device operation state. Endpoint initialization needs to occur in 
the correct preipheral state after the RESET event has been handled.


────────────────────────────────────────────────────────────────────────────────────────────────────


3. Initial Symptom

The externally visible symptom was that USB enumeration did not complete. The firmware
continued behaving as though the USB device had not successfully reached its operational
state, and the LED continued blinking instead of becoming solid. 

The LED itself was not the root cause. It was onlay an observable consequnce of the USB 
initialization/enumeraiton path failing. The investigation there had to proceed from the 
physical USB behaviour down through the interrupt system, USB status registers, endpoint
configuration, PMA configuration, and finally the ordering of operatios in the RESET
Handler.


────────────────────────────────────────────────────────────────────────────────────────────────────


4. Establishing That the Firmware was Executing

The first question was whether the firmware itself was running correctly. The device
was running the bootloader and executing normal firmware code, so the investigation
moved to the USB interrupt path.

The USB interrupt handler was associated with the STM32F103 USB low-priority interrupt:

USB_LP_CAN_RX0_IRQHandler();

The relevant vector-table entry was IRQ20, corresponding to vector-table index 36:

    Vector Index    =  16 + IRQ number
                    =  16 + 20
                    =  36

The startup assembly contained the normal Cortex-M3 vector table, with peripheral
hadler weakly aliased to Default_Handler unless explicitly overridde. 

USB_LP_CAN_RX0_IRQHandler() was explicitly implemented in usb.cpp, so the USB
interrupt had a valid route from the vector table to the firmware handler.


────────────────────────────────────────────────────────────────────────────────────────────────────


5. Proving That the USB interrupt Actually Fired

GDB was connected to the running firmware using OpenOCD:

    openocd -f interface/cmsis-dap.cfg -f target/stm32f1x.cfg

and:

    gdb-multiarch build/blue_pill_bootloader.elf

After connecting to the target:

-> target extended-remote localhost:3333
-> continue

a breakpoint was placed on the USB ISR:

-> break USB_LP_CAN_RX0_IRQHandler

GDB reported the breakpoint at the USB interrupt handle, and after continuing execution 
the breakpoint was hit:

-> Breakpoint 1, USB_LP_CAN_RX0_IRQHandler () at src/usb.cp: 120
   120      const u16 istr = USB->ISTR;

This established a critical fact that the USB peripheral was generating an interrupt
and the CPU was reaching the intended USB interrupt handler.

Therefore, the problem was not simply that the USB interrupt was disabled or that 
execution never reached the ISR.

The debugging path had successfully crossed the following hardware/software boundary:

USB peripheral -> interrupt request -> NVIC -> vector table -> USB_LP_CAN_RX0_IRQHandler()


────────────────────────────────────────────────────────────────────────────────────────────────────


6. Determining Which USB Event caused the Interrupt

The first statement in the ISR captured the USB interrupt status register:

const u16 istr = USB->ISTR;

The USB interrupt-status register is located at:

-> 0x40005C44

The RESET bit was defined as:

constexpr u16 RESET = ( 1U << 10 );

Therefore:

RESET   = bit 10
        = 0x0400

During debugging, the value of istr was inspected with:

-> print istr

and: 

-> print/x istr

Values such as:

8192 and 0x1F00 

were observed during different USB events.
More importantly, the individual event flags were tested directly:

-> print( istr & USB_ISTR_bits::RESET )

which returned :

-> 1024 or 0x0400

This proved that:

ISTR.RESET = 1

At the same time:

-> print( istr & USB_ISTR_bits::CTR )

returned:

0

Therefore, for the interrupt invocation under investigation:

    RESET  = 1
    CTR    = 0

The ISR was consequently entering the RESET-handling path rather than the normal
control-transfer(CTR) path.

This was a major narrowing of the problem.


────────────────────────────────────────────────────────────────────────────────────────────────────


7. The RESET Handler Under Investigation

The relevant code was:

if( istr & USB_ISTR_bits::RESET ) {
    
    reset_endpoints();
    USB->ISTR = ~(USB_ISTR_bits::RESET);
    return;
}

The intended purpose of the branc is straightforward: detect the USB bus reset, reinitialize the 
USB endpoint state, clear the RESET event, and return from the interrupt.

However, the operations were ordered as follows sequentially:

-> RESET detected

-> reset_endpoints()

-> clear RESET

-> return 

The corrected order is:

-> RESET detected

-> clear RESET

-> reset_endpoints()

-> return

The difference is only the ordering of these two operations, but it changes
the peripheral state in which endpoint initialization takes place.


────────────────────────────────────────────────────────────────────────────────────────────────────


8. What reset_endpoints() Was Supposed to Establish 

The endpoint-reset function contaned the following initialisation:

inline void reset_endpoints() {

    USB->BTABLE = 0;
    
    USB->EPnR[0].value = USB_EPnR_bits::EP_TYPE_CONTROL 
                       | USB_EPnR_bits::STAT_RX_VALID
                       | USB_EPnR_bits::STAT_TX_NAK;
    
    USB_BTABLE[0].ADDR_RX.data  = EP0_RX_BUFFER_OFFSET;
    
    USB_BTALBE[0].COUNT_RX.data = pma_count_rx_encode(
                                        EP0_MAX_PACKET_SIZE,
                                        PMA_BLSIZE_t::Large_32Bytes
                                    );

    USB_BTABLE[0].ADDR_TX.data  = EP0_TX_BUFFER_OFFSET;

    USB_BTABLE[0].COUNT_TX.data = 0;

    USB->DADDR = USB_DADDR_bits::EF;
}

The important endpoint configuration was:

USB_EPnR_bits::EP_TYPE_CONTORL | USB_EPnR_bits::STAT_RX_VALID | USB_EPnR_bits::STAT_RX_NAK

The individual values were

EP_TYPE_CONTROL = 0x0200
STAT_TX_VALID   = 0x3000
STAT_TX_NAK     = 0x0020

Therefore:

0x0200 | 0x3000 | 0x0020 = 0x3220

So the intended EP0 configuration value was 0x3220.

This establishes EP0 as a contorl endpoint, makes its receive side availabe  to the USB peripheral,
and leaves its transmit side in the intended NAK state.


────────────────────────────────────────────────────────────────────────────────────────────────────


9. The EP0 Register Anomaly

EP0R is located at:
  
    0x40005C00

During single-stepping through reset_endpoints(), the endpoint register was inspected with:

-> print/x *(unsigned short*)0x40005C00

The register was observed as:

    0x0000

This was unexpected because the firmware had just attemped to confiugre EP0 with:

    0x3220

After:

-> next

executin moved to the PMA configuration:

    USB_BTABLE[0].ADDR_RX.data = EP0_RX_BUFFER_OFFSET;

and EP0R was inspected again. It still appeared as:

    0x0000

This created the central debugging question:

Why was the EP0 endpoint register not exhibiting the expected configuration
even though the fimrware had executed the assignment ?

At this point several possibilities had to be considered rather than immediately
blaming the source statement.

Potential explanation included an incorrect register address, an incorrect
C++ peripheral structure, an incorrect endpoint-register stried, special hardware
semantics of EPnR, hardware ownership of particular bits, or an inappropriate peripheral
state during the write.


────────────────────────────────────────────────────────────────────────────────────────────────────


10. Verifying the USB Peripheral Register Map

The USB peripheral was represented by a C++ structure similar to :

struct USB_t {
    
    struct EPnR_t {
        vu16 value;
        u16  _unused;
    };

    std::array<EPnR__t, 8 > EPnR;
    
    std::array<u16, 16> RESERVED;

    vu16 CNTR;
    u16  _r_cntr;

    vu16 ISTR;
    u16  _r_istr;

    vu16 FNR;
    u16  _r_fnr;

    vu16 DADDR;
    u16  _r_daddr;

    vu16 BTABLE;
    u16  _r_btable;
};

The peripheral base address was:

    inline USB_t* const USB = reinterpret_cast<USB_t*>(0x40005C00U);

With each endpoint regiser occupying four bytes in the memory map, 
the expected addresses were:

    EP0R 0x40005C00 
    EP1R 0x40005C04 
    EP2R 0x40005C08 
    ... 
    EP7R 0x40005C1C 
    CNTR 0x40005C40 
    ISTR 0x40005C44 
    FNR 0x40005C48 
    DADDR 0x40005C4C 
    BTABLE 0x40005C50

The observed behaviour of the USB register was consistent with this mapping.
This substantially reduced the probability that the failure was simply caused
by an incorrect base address or incorrect endpoint-register stride.


────────────────────────────────────────────────────────────────────────────────────────────────────


11. Direct GDB Write to EP0R

To distinguish a compiler/source-code problem from a peripheral-register behaviour
problem, the endpoint register was also accessed directly through GDB.

The following was attempted:

-> set *(unsigned short*) 0x40005C00 = 0x3220

Immediately afterward:

-> print/x *(unsigned short*) 0x40005C00 

Still produced:

    0x0

This was an important observation.

It showed that the phenomenon was not merely caused by the compiler optimizing or 
transforming the C++ assignment unexpectedly. Even a direct debugger write did
not result in the expected straightforward readback.

At this point, the STM32 USB endpoint register's special hardware semantics became
important. EPnR is not an ordinary SRAM-backed regiser. Several fields have 
hardware-controlled or toggle semantics, and therefore a readback cannot automatically
be interpreted as a simple echo of every value written.

This prevented the investiation from treating:

-> write 0x3220
-> read exactly 0x3220

as the only possible definition of success.

The endpoint register had to be evaluated according to its actual hardware semantics
and, more importantly, according to whether the USB peripheral subsequently behaved 
correctly.


────────────────────────────────────────────────────────────────────────────────────────────────────


12. PMA and BTABLE Investigation

The next step was to determine whether the endpoint buffer configuration was also 
failing.

This was important because USB endpoint initialization consists of more than just
EPnR. The USB peripheral also uses the Packet Memory Aread (PMA) for USB packet
buffers, with the Buffer Table (BTABLE) describing where endpoint buffers are located.

The BTABLE itself was configured with:

    USB->BTABLE = 0;

meaning that the buffer was placed at the selected PMA base offset. The endpoint
descriptor contains the address and size information required by the USB peripheral
to locate its RX and TX buffers.


────────────────────────────────────────────────────────────────────────────────────────────────────


13. EP0 PMA Allocation

The firmware reserved space for the endpoint descriptrs and then allocated the EP0 buffers after
that region.

        PMA 
        │ 
        ├── BTABLE 
        │   ├── EP0 descriptor 
        │   ├── EP1 descriptor 
        │   ├── ... 
        │   └── EP7 descriptor 
        │ 
        ├── EP0 RX buffer 
        │ 
        └── EP0 TX buffer

The BTABLE occupies:

    8 endpoints x 8 bytes = 64 bytes

so:

    constexpr u32 BTABLE_LOCAL_SIZE = ( 8 * 8 );

produces 64 bytes.

The EP0 buffer offsets were then calculate form the reserved BTABLE space
and the configured buffer sizes.


────────────────────────────────────────────────────────────────────────────────────────────────────


14. PMA/BTABLE Values Were Successfully Written

The PMA descriptro was subsequently inspected direclty.

The observed values included:

ADDR_RX     = 128
COUNT_RX    = 64
ADDR_TX     = 192
COUNT_TX    = 0

These values were consistent with the endpoint-buffer allocation being performed
by reset_endpoints().

This was a significant result because it demonstrated that the PMA writes were actually
reaching the expected memory.

The debugging therefor narrowed the failure further:

EP0R configuration          -> anamolous
BTABLE configuration        -> written correctly
PMA buffer configuration    -> written correctly

This made a general USB-memory addressing failure much less likely.


────────────────────────────────────────────────────────────────────────────────────────────────────


15. Verifying DADDR

The final part of reset_endpoints() configured:

    USB->DADDR  = USB_DADDR_bits::EF;

The USB device-address register is located at:

    0x40005C4C

After execution, it was inspected with GDB.
The observed value was:

    0x80

which corresponds to:

    EF = bit 7

Therefor the USB device enable bit was being accepted.

This was another important isolation point. The USB peripheral was not 
generally refusing all configuration writes.

The evidence up until this point were as follows:

USB ISTR            -> readable
USB RESET           -> correctly detected
USB interrupt       -> reaches CPU
PMA/BTABLE          -> writable
DADDR               -> writable
EP0R                -> writes are being ignored 

This investigation therefore had to focus specifically on the relationship
between USB RESET handling and endpoint reinitialization.


────────────────────────────────────────────────────────────────────────────────────────────────────


16. Understanding CTR_RX and SETUP

Once the RESET path was isolated, the next quetion was whether the device could actually 
receive a USB contorl transaction.

The endpoint register contains hardware-generated transaciton status information 
including:

    CTR_RX = ( 1U << 15 );

and:
    SETUP  = ( 1U << 11 );

These bits are not equivalent to ordinary software configuraiton fields. 

A real USB SETUP transaction must occur on the physical USB bus. The USB serial interface
engine (SIE) receives that transaction and updates the endpoint status accordingly.

The expected sequence is as follows:

-> Host sends SETUP packet

-> USB SIE receives packet

-> EP0 receive transaction compeletes

-> EP0R.CTR_RX becomes asserted

-> ISTR.CTR becomes asserted

-> USB ISR handles CTR

-> SETUP bit is examined

The distincation became importatnt during the debugging because attempting to simulate
a CORRECT TRANSFER for the SETUP thorugh debugger writes is not equivalent to causing the 
hardware to receive an actual USB transaction.


────────────────────────────────────────────────────────────────────────────────────────────────────


17. Why Artificially Forcing CTR Was Not a Valid USB Test

Attempts were made to understand whether hardware status bits such as CTR_RX and SETUP
could be written manually.

However, these fields represent transaction state generate by the USB peripheral. A debugger
write does not reproduce the physical sequence:

USB host -> USB signaling -> USB SIE -> endpoint packet reception -> transaction completion -> CTR_RX

Similarly, forcing the NVIC pending state would only test whether the CPU can enter the ISR. It would
not create a USB packet.

Therefore, after the RESET-path problem was identified, the meaningful verification had to return 
to the real USB bus:

USB host -> physical SETUP transaction -> STM32 USB peripheral -> EP0 status -> ISR


────────────────────────────────────────────────────────────────────────────────────────────────────


18. Relationship Between RESET and CTR



