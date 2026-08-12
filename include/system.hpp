/************************************************************
 *
 * system.hpp
 *
 * Clock Initialization and Millisecond Timing.
 *
 ***********************************************************/
 
#pragma once
#include "stm32f103.hpp"

// =================================================================
// Configure PLL for 48 MHz SYSCLK from HSI.
// After return:
//      -> SYSCLK   = HCLK = APB2 = 48 MHz
//      -> APB1     = 24 MHz
//      -> USB      = 48 MHz  (via PLL, bypassing APB1 limit)
// =================================================================
void clock_init();


// =================================================================
// Start SysTick at 1 ms period. Requires clock_init() first.
// Enables the SysTick exception: SysTick_Handler fires every 1ms.
// =================================================================
void systick_init();


// =================================================================
// Return milliseconds elapsed since systick_init().
// Wraps after 2^32 ms ≈ 49.7 days.
// =================================================================
u32 get_tick_ms();


// =================================================================
// Busy-Wait for exactly ms milliseconds.
// Requires systick_init(). Minimum granularity: 1 ms.
// =================================================================
void delay_ms( u32 ms );


/* This is the END. */