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

// =================================================================
// SysTick_Handler
// 
// extern "C": supresses C++ name mangling.
// 
// This is a strong definition: it overrides the weak .thumb_set alias
// in startup.s. The linker prefers strong definitions over weak ones.
// =================================================================
extern "C" void SysTick_Handler(void){
    g_ms++;
}

u32 get_tick_ms(){
    return g_ms;
}

