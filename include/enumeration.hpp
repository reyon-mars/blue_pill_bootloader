#pragma once
#include "stm32f103.hpp"

extern const u8 g_device_descriptor[18];

struct SetupPacket_t {
  u8 bmRequestType;
  u8 bRequest;
  u16 wValue;
  u16 wIndex;
  u16 wLength;
};

static_assert(sizeof(SetupPacket_t) == 8, "SetupPacket_t size mismatch");

extern SetupPacket_t g_pending_setup;

SetupPacket_t read_setup_packet();

void handle_setup_request(const SetupPacket_t &setup);

namespace UsbRequest {
constexpr u8 GET_DESCRIPTOR = 6;
constexpr u8 SET_ADDRESS = 5;
} // namespace UsbRequest

namespace UsbDescriptorType {
constexpr u8 DEVICE = 1;
}
