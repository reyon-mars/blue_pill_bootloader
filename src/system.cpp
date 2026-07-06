#include "../include/system.hpp"

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
    //      "The reset state of RCC_CFGR is also guaranteed to be 0x00000000
    //       (all set to zeroes)."
    //
    // Configuration rationale:
    //  PLLSRC_HSI_DIV2: PLL input = 8 MHz / 2 = 4 MHz.
    //      (HSE is not populated on most Blue Pill; must use HSI/2.)
    //      The /2 is fixed in hardware, HSI cannot feed PLL directly.
    //  PLLMUL_12: 4 MHz x 12 = 48 MHz PLL output.
    //  USBPRE_DIV1: USB clock = PLL / 1 = 48 MHz.
    //      (Use DIV1.5 only when PLL = 72 MHz: 72/1.5 = 48 MHz.)  
    //  HPRE_DIV1: HCLK = SYSCLK = 48 MHz.
    //  PPRE1_DIV2: APB1 = HCLK / 2 = 24 MHz.
    //      REQUIRED: RM0008 specifies APB1 maximum = 36 MHz.
    //      Exceeding this violates timing specs of I2C, SPI2, etc.
    //      24 MHz gives comfortable margin below the 36 MHz limit.
    //  PPRE2_DIV1: APB2 = HCLK = 48 MHz. GPIO, SPI1, USART1 are here.  
    // ─────────────────────────────────────────────────────────────
    RCC->CFGR = RCC_CFGR_bits::PLLSRC_HSI_DIV2
              | RCC_CFGR_bits::PLLMUL_12
              | RCC_CFGR_bits::USBPRE_DIV1
              | RCC_CFGR_bits::HPRE_DIV1
              | RCC_CFGR_bits::PPRE1_DIV2
              | RCC_CFGR_bits::PPRE2_DIV1;
              
    // ─────────────────────────────────────────────────────────────
    // STEP 3: Enable the PLL.
    //
    // We need to use |= here, instead of a full assignment because
    // the HSION (bit 0) controls the HSI oscillator. If we wrote
    // a full assignment with HSION = 0, we would turn off HSI, which
    // is the clock source we are CURRENTLY RUNNING FROM, before the
    // PLL is locked. The chip would lose its clock entirely.
    // ─────────────────────────────────────────────────────────────
    RCC->CR |= RCC_CR_bits::PLLON;
    


}