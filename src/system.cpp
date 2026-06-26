#include "../include/system.h"

// =================================================================
// INTERNAL STATE
//
// g_ms is incremented by SysTick_Handler once every 1 ms.
// It is the global millisecond reference for the bootloader.
// Used by: delay_ms(), get_tick_ms(), and DFU timeout.
// =================================================================
namespace {
    volatile u32 g_ms = 0;
}