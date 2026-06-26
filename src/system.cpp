#include "../include/system.h"

// =================================================================
// INTERNAL STATE ( g_ms )
//
// g_ms is incremented by SysTick_Handler once every 1 ms.
// It is the global millisecond reference for the bootloader.
// Used by: delay_ms(), get_tick_ms(), and DFU timeout.
// =================================================================
namespace {
    volatile u32 g_ms = 0;
}

// =================================================================
// SysTick_Handler()
// 
// extern "C": supresses C++ name mangling.
// 
// This is a strong definition: it overrides the weak .thumb_set alias
// in startup.s. The linker prefers strong definitions over weak ones.
// =================================================================
extern "C" void SysTick_Handler(void){
    g_ms++;
}

inline u32 get_tick_ms(){
    return g_ms;
}


// =================================================================
// delay_ms()
// =================================================================
void delay_ms(u32 ms){
    const u32 start = g_ms;
    
    while( (g_ms-start) < ms ){
        // g_ms is volatile: reloaded from SRAM every iteration.
        // SysTick_Handler increments g_ms once per ms via interrupt.
    }
}


// =================================================================
// clock_init()
//
// Configures 