#include "../include/usb.hpp"
#include "../include/system.hpp"
#include <cstddef>


// =================================================================
// Global USB SETUP seen Flag
// 
// Records whether a valid SETUP packet has been received on endpoint
// 0. Set upon receiving the first valid SETUP transaction and remains
// true until explicitly cleared by firmware.
// =================================================================
volatile bool g_usb_setup_seen = false;


namespace {

    // =================================================================
    // reset_endpoints()
    // 
    // Initializes the USB Buffer Table, EP0 endpoint state, and USB
    // device address after power-up or a USB bus reset. The BTABLE
    // is placed at PMA-local offset 0, EP0 is configured as a control
    // endpoint, its RX and TX PMA buffers are described in the BTABLE,
    // and the device is finally enabled with USB address 0.
    // The BTABLE and EP0 must be fully configured before DADDR.EF is 
    // set. This keeps the USB peripheral from being enabled until the 
    // endpoint configuration and its PMA buffer descriptors have been
    // established.
    // =================================================================
    inline void reset_endpoints() {

        // ───────────────────────────────────────────────────────────────
        // Set the BTABLE base to PMA-local offset 0. The USB peripheral
        // interprets BTABLE using its PMA-local addressing convention,
        // so endpoint 0's descriptor begins at the start of the PMA.
        // ───────────────────────────────────────────────────────────────
        USB->BTABLE = 0;

        // ───────────────────────────────────────────────────────────────
        // Initializes EP0 as a control endpoint. RX is made VALID so the 
        // SIE can accept an incoming control packet, while TX is initially
        // NAK so the SIE does not transmit until firmware has prepared
        // data.
        // ───────────────────────────────────────────────────────────────
        USB->EPnR[0].value = USB_EPnR_bits::EP_TYPE_CONTROL
                           | USB_EPnR_bits::STAT_RX_VALID
                           | USB_EPnR_bits::STAT_TX_NAK;

        // ───────────────────────────────────────────────────────────────
        // Set the PMA-local starting offset of the EP0 RX buffer in its
        // BTABLE descriptor. This is a PMA-local offset, not CPU address.
        // ───────────────────────────────────────────────────────────────
        USB_BTABLE[0].ADDR_RX.data = EP0_RX_BUFFER_OFFSET;

        // ───────────────────────────────────────────────────────────────
        // Describe the maximum capacity of the EP0 RX buffer using the 
        // hardware-defined COUNT_RX encoding. EP0 has a 64-byte RX buffer,
        // allocated using the 32-byte block encoding.
        // ───────────────────────────────────────────────────────────────
        USB_BTABLE[0].COUNT_RX.data = pma_count_rx_encode(
                                            EP0_MAX_PACKET_SIZE,
                                            PMA_BLSIZE_t::Large_32Bytes
                                        );

        // ───────────────────────────────────────────────────────────────
        // Set the PMA-local starting offset of the EP0 TX buffer. The TX
        // buffer is located immediately after the EP0 RX buffer in PMA.
        // ───────────────────────────────────────────────────────────────
        USB_BTABLE[0].ADDR_TX.data = EP0_TX_BUFFER_OFFSET;

        // ───────────────────────────────────────────────────────────────
        // EP0 has no transmit data prepared at initialization. COUNT_TX
        // is cleared to zero; firmware sets it when an EP0 IN transfer has
        // been prepared for transmission.
        // ───────────────────────────────────────────────────────────────
        USB_BTABLE[0].COUNT_TX.data = 0;

        // ───────────────────────────────────────────────────────────────
        // Enable the USB device by setting DADDR.EF. The ADDR field 
        // remains zero, which is the USB device's initial address before 
        // the host assigns a new address through the SET_ADDRESS control
        // request.
        // ───────────────────────────────────────────────────────────────
        USB->DADDR = USB_DADDR_bits::EF;
    }


    // =================================================================
    // ep_clear_ctr_rx()
    //
    // Clears the EPnR CTR_RX (Correct-Transfer RX) flag while preserving
    // the endpoint configuration and leaving all other hardware-controlled
    // EPnR fields unchanged according to their write semantics.
    // =================================================================
    inline void ep_clear_ctr_rx( EP_Num_t ep_num ) {

        // Read EPnR state exactly once.
        const u16 read_state = USB->EPnR[ static_cast<size_t>(ep_num) ].value;
        
        // Preserve the ordinary read/write fields by copying their values
        // from the read_state. These fields are not modified by this function.
        const u16 preserved  = read_state & (  USB_EPnR_bits::EP_TYPE_MASK
                                             | USB_EPnR_bits::EP_KIND
                                             | USB_EPnR_bits::EA_MASK );
        
        // Write-1 to CTR_TX to preserve its status; leave CTR_RX as 0 to 
        // clear it (rc_w0).
        USB->EPnR[ static_cast<size_t>(ep_num) ].value = preserved
                                                       | USB_EPnR_bits::CTR_TX;
    }
}


// =================================================================
// USB_LP_CAN_RX0_IRQHandler()
// =================================================================
extern "C" void USB_LP_CAN_RX0_IRQHandler() {

    const u16 istr = USB->ISTR;

    if( istr & USB_ISTR_bits::RESET ) {

        reset_endpoints();
        USB->ISTR = ~(USB_ISTR_bits::RESET);
        return;
    }

    if( istr & USB_ISTR_bits::CTR ) {
        const u16 ep_id = istr & USB_ISTR_bits::EP_ID_MASK;
        if( ep_id == 0 ) {
            if( USB->EPnR[0].value & USB_EPnR_bits::SETUP )
            {
                g_usb_setup_seen = true;
            }
            ep_clear_ctr_rx(EP_Num_t::EP0);
        }
    }
}


// =================================================================
// pma_write_word()
//
// Writes 16-bit to the USB Packet Memory Area at local_offset.
// =================================================================
void pma_write_word( u32 local_offset, u16 value ) {
    USB_PMA[ local_offset >> 1 ].data = value;
}


// =================================================================
// pma_read_word()
//
// Reads 16-bit  from the USB Packet Memory Area at local_offset.
// =================================================================
u16 pma_read_word( u32 local_offset ) {
    return USB_PMA[ local_offset >> 1 ].data;
}


// =================================================================
// pma_write_words()
//
// Writes a contiguous sequence of 16-bit words to USB PMA.
// =================================================================
void pma_write_words( u32 local_offset, std::span<const u16> src ) {
    
    local_offset >>= 1;

    for( const auto& value : src) {
        USB_PMA[local_offset].data = value;
        local_offset++;
    }
}


// ===================================================================
// ep_set_status()
//
// RM0008 warns that an ordinary read-modify-write on EPnR is unsafe,
// because the SIE can change hardware-controlled fields at any time, 
// including the exact instant between our read and subsequent write.
// If we simply copied the values read from EPnR back into the register,
// the write could contain stale values for hardware-controlled bits 
// and accidentally overwrite changes made by the SIE after our read.
//
// To avoid this, EPnR is read exactly once, and the write value is 
// constructed explicitly from the read_state. Each field is handled
// according to its hardware-defined write semantics:
// 
//  -> CTR_RX, CTR_TX   : Written as 1. These are rc_w0 bits, where
//                        writing 1 means "leave unchanged" and 
//                        writing 0 means "clear". This function never
//                        intentionally clears a completion flag. 
//
//  -> STAT_TX, STAT_RX : Toggle-on-write fields. Writing a 1 inverts
//                        a bit's current state, while writing 0 leaves
//                        it unchanged. To transition the hardware to a 
//                        desired state, we XOR the read value with the
//                        target value to produce the toggle mask.
//
//  -> EP_TYPE/KIND/EA  : Ordinary read/write fields. Their values are
//                        copied from the read_state so that changing
//                        the status fields does not unintentionally
//                        modify these fields.
// ===================================================================
void ep_set_status( EP_Num_t ep_num, u16 new_stat_tx, u16 new_stat_rx ) {
    
    // Read EPnR state exactly once.
    const u16 read_state = USB->EPnR[ static_cast<size_t>(ep_num) ].value;
    
    // Both STAT_RX and STAT_TX use toggle-on-write semantics.
    // XOR identifies exactly which bits differ between the current 
    // and the desired state; those differing bits must toggle.
    const u16 toggle_rx = (read_state ^ new_stat_rx) & USB_EPnR_bits::STAT_RX_MASK;
    const u16 toggle_tx = (read_state ^ new_stat_tx) & USB_EPnR_bits::STAT_TX_MASK;

    // Preserve the ordinary read/write fields by copying their values
    // from the read_state. These fields are not modified by this function.
    const u16 preserved = read_state & (  USB_EPnR_bits::EP_TYPE_MASK 
                                        | USB_EPnR_bits::EP_KIND      
                                        | USB_EPnR_bits::EA_MASK );

    // Construct the complete EPnR write value.
    // CTR_RX/CTR_TX are written as 1 so rc_w0 semantics leave them
    // unchanged. toggle_tx/toggle_rx contain only the status-bit
    // toggles required to reach the desired state.
    USB->EPnR[ static_cast<size_t>(ep_num) ].value =  preserved
                                                    | USB_EPnR_bits::CTR_RX
                                                    | USB_EPnR_bits::CTR_TX
                                                    | toggle_rx
                                                    | toggle_tx;
}


// =========================================================================
// usb_init()
//
// Initializes the USB peripheral from a known reset state to an operational
// state. The sequence first enables the USB peripheral clock, then forces 
// the USB digital Serial Interface Engine (SIE) into reset so that its 
// internal state machines and endpoint state can be brought to a known 
// state. After a short delay, the forced reset is released and any stale USB
// interrupt status is cleared. The endpoint registers are then initialized 
// to their required initial configuration. Finally, the USB RESET and Correct
// Transfer (CTR) interrupt sources are enabled so that subsequent bus reset
// and endpoint-transfer events can be reported to the CPU.
// =========================================================================
void usb_init() {
    // ───────────────────────────────────────────────────────────────
    // STEP 1: Enable the USB peripheral clock.
    // 
    // USBEN controls the clock supplied to the USB peripheral through
    // the APB1 peripheral clock domain. Until this clock is enabled,
    // the USB register interface and the peripheral's internal logic
    // cannot be operated normally.
    //
    // This write needs to occur before accessing the USB peripheral's
    // control or status register.
    // ───────────────────────────────────────────────────────────────
    RCC->APB1ENR |= RCC_APB1ENR_bits::USBEN;

    // ───────────────────────────────────────────────────────────────
    // STEP 2: Power the Analog Transceiver; hold digital SIE in reset.
    //
    // Power-on default of CNTR is 0x0003 (PDWN=1, FRES=1).
    // Writing CNTR = FRES (0x0001) clears PDWN to power up the analog
    // Transceiver, while asserting reset on the Digital SIE.
    // ───────────────────────────────────────────────────────────────
    USB->CNTR = USB_CNTR_bits::FRES;

    // ───────────────────────────────────────────────────────────────
    // STEP 3: Propagate reset and await analog stabilization.
    // 
    // The delay gives the analog Transceiver power rail time to 
    // stabilize and for the digital SIE state logic to fully settle
    // in the forced-reset state before software releases reset and 
    // continues initialization.
    // ───────────────────────────────────────────────────────────────
    delay_ms( 1 );

    // ───────────────────────────────────────────────────────────────
    // STEP 4: Release the digital SIE from forced reset.
    //
    // Writing CNTR = 0 clears FRES (releasing the SIE) while keeping
    // PDWN = 0 (transceiver powered on). All interrupt masks remain 
    // disabled at this stage so the CPU won't handle IRQs during 
    // setup.
    // ───────────────────────────────────────────────────────────────
    USB->CNTR = 0;

    // ───────────────────────────────────────────────────────────────
    // STEP 5: Clear all pending interrupt status flags.
    //
    // USB_ISTR flags are rc_w0 bits set autonomously by the hardware.
    // Writing 0 clears all pending flags accumulated during power-up
    // or transceiver startup, ensuring a clean state before active
    // operation.
    // ───────────────────────────────────────────────────────────────
    USB->ISTR = 0;

    // ───────────────────────────────────────────────────────────────
    // STEP 6: Configure BTABLE offsets and Endpoint layout.
    //
    // Calls reset_endpoints() to assign BTABLE at offset 0, set up
    // EP0 RX/TX descriptors in PMA SRAM, set EP0 as CONTROL with 
    // STAT_RX_VALID / STAT_TX_NAK, and enable the device.
    // ───────────────────────────────────────────────────────────────
    reset_endpoints();

    // ───────────────────────────────────────────────────────────────
    // STEP 7: Enable USB RESET and Correct Transfer (CTR) interrupts.
    //
    // Unmasks core USB interrupt sources by setting RESETM and CTRM
    // in CNTR. This enables host-driven bus reset detection and
    // endpoint transfer completion events to escalate into CPU NVIC
    // interrupts.
    // ───────────────────────────────────────────────────────────────
    USB->CNTR = USB_CNTR_bits::RESETM | USB_CNTR_bits::CTRM;

    // ───────────────────────────────────────────────────────────────
    // STEP 8: Enable the USB interrupt line in the NVIC.
    //
    // USB->CNTR enables the USB peripheral to generate interrupt
    // requests, but those requests cannot reach the CPU until the 
    // corresponding interrupt channel is also enabled in the NVIC.
    // This is done last so the complete USB interrupt configuration 
    // is already valid before the CPU can service an interrupt.
    // ───────────────────────────────────────────────────────────────
    NVIC_helpers::enable_irq(
        static_cast<int>(IRQn::USB_LP_CAN_RX0)
    );
}