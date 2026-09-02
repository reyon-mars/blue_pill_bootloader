#include "../include/enumeration.hpp"
#include "../include/usb.hpp"


SetupPacket_t g_pending_setup{};

const u8 g_device_descriptor[18] = {
    18,
    1,
    0x10, 0x01,
    0,
    0,
    0,
    64,
    0x09, 0x12,
    0x01, 0x00,
    0x00, 0x01,
    0,
    0,
    0,
    1
};


SetupPacket_t read_setup_packet() {

    const u16 w0 = pma_read_word( EP0_RX_BUFFER_OFFSET + 0 );
    const u16 w1 = pma_read_word( EP0_RX_BUFFER_OFFSET + 1 );
    const u16 w2 = pma_read_word( EP0_RX_BUFFER_OFFSET + 2 );
    const u16 w3 = pma_read_word( EP0_RX_BUFFER_OFFSET + 3 );
    
    SetupPacket_t packet;

    packet.bmRequestType    = static_cast<u8>( w0 & 0xFFU );
    packet.bRequest         = static_cast<u8>( (w0 >> 8) & 0xFFU);
    packet.wValue           = w1;
    packet.wIndex           = w2;
    packet.wLength          = w3;

    return packet;
}
