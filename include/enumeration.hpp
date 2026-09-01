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

 namespace UsbRequest {
   constexpr u8 GET_DESCRIPTOR   = 6;
   constexpr u8 SET_ADDRESS      = 5;   
 }

 namespace UsbDescriptorType {
   constexpr u8 DEVICE  = 1;
 }
