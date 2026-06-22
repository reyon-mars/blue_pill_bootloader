// include/stm32f103.h
//
// Raw register definitions for STM32F103C8T6.
//
// Struct-based peripheral mapping
//

#pragma  once
#include <cstdint>


// Type aliases

using u8    = uint8_t;
using u16   = uint16_t;
using u32   = uint32_t;
using vu32  = volatile uint32_t; // For hardware registers

// =================================================================
// RCC - Reset and Clock Control
// Base address: 0x40021000
//
// Every peripheral on the STM32 is gated behind a clock. The peripheral
// is POWERED OFF by default. The clock for the peripheral must be enabled
// in RCC before the first register write to it, or the write is silently 
// ignored  ( the APB bus returns all-ones on reads, writes have no effect).
// =================================================================

struct RCC_t {
    vu32 CR;                // 0x00 Clock Control: enable HSI/HSE/PLL, read ready flags
    vu32 CFGR;              // 0x04 Clock Config: select system clock, set PLL, dividers
    vu32 CIR;               // 0x08 Clock Interrupt: ready flags, enable/clear interrupts
    vu32 APB2RSTR;          // 0x0C APB2 Peripheral reset register
    vu32 APB1RSTR;          // 0x10 APB1 Peripheral reset register
    vu32 AHBENR;            // 0x14 AHB Peripheral clock enable ( DMA, SRAM, CRC....)
    vu32 APB2ENR;           // 0x18 APB2 Clock Enable (GPIOA-E, ADC, SPI1, USART1...)
    vu32 APB1ENR;           // 0x1C APB1 Clock Enable (USB, TIM2-7, I2C, USART2-3...)
    vu32 BDCR;              // 0x20 Backup Domain control
    vu32 CSR;               // 0x24 Control/Status (low-power reset flag )
};

// Compile-time layout verification.
// If this fails, the struct fields are wrong ( wrong type, wrong count, padding ).
static_assert(sizeof(RCC_t)== 40, "RCC_t size mismatch - check register definitions");

// The single RCC peripheral instance. 'static' means this symbol is 
// not exported to other translation units (avoids link-time collisions 
// when multiple .cpp files include this header ).
static RCC_t* const RCC = reinterpret_cast<RCC_t*>(0x40021000U);

// RCC_CR bit definitions
namespace RCC_CR_bits {
    constexpr u32 HSION     = ( 1U << 0 );      // Enable internal 8MHz HSI oscillator
    constexpr u32 HSIRDY    = ( 1U << 1 );      // HSI ready flag ( hardware sets this; READ ONLY )
    constexpr u32 HSEON     = ( 1U << 16 );     // Enable external crystal (HSE)
    constexpr u32 HSERDY    = ( 1U << 17 );     // HSE ready flag ( READ ONLY )
    constexpr u32 PLLON     = ( 1U << 24 );     // Enable PLL
    constexpr u32 PLLRDY    = ( 1U << 25 );     // PLL locked flag ( READ ONLY )
}

// RCC_CFGR bit definitions
namespace RCC_CFGR_bits {
    // SW[1:0] - System clock switch
    constexpr u32 SW_HSI    = ( 0x0U << 0 );    // Use HSI as SYSCLK
    constexpr u32 SW_HSE    = ( 0x1U << 0 );    // Use HSE as SYSCLK
    constexpr u32 SW_PLL    = ( 0x2U << 0 );    // Use PLL output as SYSCLK
    // SWS[1:0] - Current Clock Status ( READ-ONLY, reflects what's actually running )
    constexpr u32 SWS_MASK  = ( 0x3U << 2 );
    constexpr u32 SWS_PLL   = ( 0x2U << 2 );
    // AHB prescaler: HPRE[3:0] at bits 7:4
    constexpr u32 HPRE_DIV1 = ( 0x0U << 4 );    // HCLK = SYSCLK ( no division )
    // APB1 prescaler (max 36MHz): PPRE1[2:0] at bits 10:8
    constexpr u32 PPRE1_DIV1 = ( 0x0U << 8 );   // APB1 = HCLK
    constexpr u32 PPRE1_DIV2 = ( 0x4U << 8 );   // APB1 = HCLK/2
    // APB2 prescaler: PPRE2[2:0] at bits 13:11
    constexpr u32 PPRE2_DIV1 = ( 0x0U << 11 );  // APB2 = HCLK
    // PLL source: PLLSRC bit 16
    constexpr u32 PLLSRC_HSI_DIV2   = ( 0U << 16 );    // PLL input = HSI/2 = 4MHz
    constexpr u32 PLLSRC_HSE        = ( 1U << 16 );    // PLL input = HSE
    // PLL multiplier: PLLMUL[3:0] at bits 21:18
    // Value offset of +2, i.e. count starts from 2 and not 0 or 1
    // Value 0b1010 = 10 decimal -> multiply by 12
    // 4MHz (HSI/2) x 12 = 48MHz -> perfect for USB
    constexpr u32 PLLMUL_12 = ( 0xAU << 18 );
    // USB prescaler: USBPRE bit 22
    // 1 = PLL not divided ( USB clock = PLL = 48MHz when PLL = 48MHz )
    constexpr u32 USBPRE_DIV1 = ( 1U << 22 );    
}


// RCC_APB2ENR bit definitions
namespace RCC_APB2ENR_bits {
    constexpr u32 AFIOEN    = ( 1U << 0 );  // Alternate Function I/O clock
    constexpr u32 IOPAEN    = ( 1U << 2 );  // GPIOA clock enable
    constexpr u32 IOPBEN    = ( 1U << 3 );  // GPIOB clock enable
    constexpr u32 IOPCEN    = ( 1U << 4 );  // GPIOC clock enable
    constexpr u32 IOPDEN    = ( 1U << 5 );  // GPIOD clock enable
    constexpr u32 SPI1EN    = ( 1U << 12 ); // SPI1  clock enable
}

// RCC_APB1ENR bit definitions
namespace RCC_APB1ENR_bits {
    constexpr u32 TIM2EN    = ( 1U << 0 );  // TIM2 clock enable
    constexpr u32 I2C1EN    = ( 1U << 21 ); // I2C1 clock enable
    constexpr u32 USBEN     = ( 1U << 23 ); // USB  clock enable
}


// =================================================================
// GPIO - General Purpose Input/Output
// GPIOA Base: 0x40010800
// GPIOC Base: 0x40011000
//
// Each GPIO pin is described by 4 bits in CRL (pins 0-7) or CRH (pins 8-15).
// The 4 bits are [CNF1:CNF0:MODE1:MODE0]:
// 
// MODE[1:0]:
//      00 = INPUT
//      01 = OUTPUT 10MHz
//      10 = OUTPUT 2MHz
//      11 = OUTPUT 50MHz 
//
// CNF[1:0] when output ( MODE != 00 )
//      00 = PUSH-PULL
//      01 = OPEN-DRAIN
//      10 = ALTERNATE FUNCTION PUSH-PULL
//      11 = ALTERNATE FUNCTION OPEN-DRAIN
//
// CNF[1:0] when input ( MODE == 00 )
//      00 = ANALOG
//      01 = FLOATING
//      10 = PULL-UP/DOWN
//      11 = RESERVED
// =================================================================
struct GPIO_t {
    vu32
};