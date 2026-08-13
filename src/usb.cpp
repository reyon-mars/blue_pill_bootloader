#include "../include/usb.hpp"

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


