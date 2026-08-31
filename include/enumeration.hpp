 #pragma once
 #include "stm32f103.hpp"
 
 
 struct SetupPacket_t {
    u8  bmRequestType;
    u8  bRequest;
    u16 wValue;
    u16 wIndex;
    u16 wLength;
 };

 static_assert( sizeof(SetupPacket_t) == 8, "SetupPacket_t size mismatch");

 extern SetupPacket_t g_pending_setup;
 
 SetupPacket_t read_setup_packet();
