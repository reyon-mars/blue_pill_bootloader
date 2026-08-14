#include "../include/usb.hpp"
#include <cstddef>

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