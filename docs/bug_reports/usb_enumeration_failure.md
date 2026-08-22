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




