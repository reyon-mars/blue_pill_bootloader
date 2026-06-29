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
// Configures SYSCLK = 48 MHz from HSI via PLL.
//
// Starting state (guaranteed by hardware reset):
//      -> SYSCLK = HSI = 8 MHz
//      -> PLL = OFF
//      -> AHB/APB prescalers = /1
//      -> FLASH wait states  = 0
//      -> All Peripheral Clocks = OFF (except RCC, SRAM, FLASH)
// =================================================================
void clock_init(){
    
    // ─────────────────────────────────────────────────────────────
    // STEP 1: FLASH wait states MUST come before any Clock increase.
    //
    // Currently at 8 MHz: 0 wait states are correct.
    // After switch to 48 MHz: 1 wait state required.
    // We need to configure wait states at 8 MHz, because this write
    // itself requires 0 wait states to execute correctly and is in 
    // place before the frequency rises.
    //
    // If we raised frequency first then set wait states:
    //  -> The fetch of THIS very instruction would be corrupted.
    //  -> We would likely never reach the wait state configuration.
    //
    // ACR_PRFTBE: enable 2-word (64 Bytes) prefetch buffer. Speculatively 
    // reads next word while CPU executes current instruction. Recovers most 
    // of the wait state throughput penalty for sequential code.
    // ─────────────────────────────────────────────────────────────
    FLASH->ACR  =   FLASH_bits::ACR_LATENCY_1WS | FLASH_bits::ACR_PRFTBE;
    
    // ─────────────────────────────────────────────────────────────
    // STEP 2: Configure CFGR before enabling PLL.
    // 
    // RM0008 Section 7.3.2 states explicitly:
    //      "PLLSRC and PLLMUL bits can be written only when PLL is 
    //       disabled (PLLON = 0)."
    //      "The reset state of CFGR is also guaranteed to be 0x00000000
    //       (all zeros)."
    //



}