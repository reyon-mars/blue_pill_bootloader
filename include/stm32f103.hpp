/************************************************************
 * 
 * stm32f103.h
 *
 * Raw register definitions for STM32F103C8T6.
 *
 * Struct-based peripheral mapping
 *
 ***********************************************************/

#pragma  once
#include <array>
#include <cstdint>


// Type aliases
using u8    = uint8_t;
using u16   = uint16_t;
using u32   = uint32_t;
using vu8   = volatile uint8_t;     // For hardware registers
using vu16  = volatile uint16_t;    // For hardware registers
using vu32  = volatile uint32_t;    // For hardware registers


// =================================================================
// SCB - System Control Block (ARM Cortex-M3 core peripheral)
// Base address: 0xE000ED00
// 
// Not an ST peripheral, it is the part of the ARMv7-M architecture 
// itself, byte-identical on every Cortex-M3 from any vendor. Governs
// exception behaviour, reports fault causes, and holds VTOR (Vector
// Table Offset Register).
// =================================================================
struct SCB_t {
    vu32 CPUID;     // 0x00 CPU ID
    vu32 ICSR;      // 0x04 Interrupt Control and State (pending exception info)
    vu32 VTOR;      // 0x08 Vector Table Offset
    vu32 AIRCR;     // 0x0C App Interrupt and Reset Control
    vu32 SCR;       // 0x10 System Control (sleep-on-exit, sleep-deep, wake-on-pending)
    vu32 CCR;       // 0x14 Configuration and Control (stack alignment, div-by-zero trap)
    vu32 SHPR1;     // 0x18 System Handler Priority 1 (MemManage, BusFault, UsageFault)
    vu32 SHPR2;     // 0x1C System Handler Priority 2 (SVCall)
    vu32 SHPR3;     // 0x20 System Handler Priority 3 (PendSV, SysTick)
    vu32 SHCSR;     // 0x24 System Handler Control & State (enable/ pending/ active bits)
    vu32 CFSR;      // 0x28 Configurable Fault Status (MemManage + BusFault + UsageFault causes)
    vu32 HFSR;      // 0x2C HardFault Status (escalation cause)
    vu32 DFSR;      // 0x30 Debug Fault Status
    vu32 MMFAR;     // 0x34 MemManage Fault Address (valid only if MMARVALID set in CFSR)
    vu32 BFAR;      // 0x38 BusFault Address (valid only if BFARVALID set in CFSR)
    vu32 AFSR;      // 0x3C Auxiliary Fault Status
};

// Compile-time layout verification.
// If this fails, the struct fields are wrong ( wrong type, wrong count, padding ).
static_assert(sizeof(SCB_t) == 64, "SCB_t size mismatch");

// The single SCB peripheral instance. 'static' means this symbol is 
// not exported to other translation units (avoids link-time collisions 
// when multiple .cpp files include this header ).
static SCB_t* const SCB = reinterpret_cast<SCB_t*>(0xE000ED00U);

// SCB_CCR bit definitions
namespace SCB_CCR_bits{
    constexpr u32 STKALIGN      = ( 1U << 9 );    // Force 8-byte stack alignment on exception entry
    constexpr u32 DIV_0_TRP     = ( 1U << 4 );    // 1 = integer divide-by-zero raises UsageFault
    constexpr u32 UNALIGN_TRP   = ( 1U << 3 );    // 1 = unaligned access raises UsageFault
}

// SCB_CFSR bit definitions
namespace SCB_CFSR_bits{
    // Byte 0 = MemManage status, Byte 1 = BusFault status, Byte 2:3 = UsageFault status
    constexpr u32 IACCVIOL      = ( 1U << 0 );      // MemManage:   instruction access violation
    constexpr u32 DACCVIOL      = ( 1U << 1 );      // MemManage:   data access violation
    constexpr u32 MMARVALID     = ( 1U << 7 );      // MemManage:   MMFAR holds a valid faulting address
    constexpr u32 IBUSERR       = ( 1U << 8 );      // BusFault:    instruction bus error
    constexpr u32 PRECISERR     = ( 1U << 9 );      // BusFault:    precise data bus error (BFAR valid)
    constexpr u32 IMPRECISEERR  = ( 1U << 10 );     // BusFault:    imprecise data bus error (BFAR NOT valid)
    constexpr u32 BFARVALID     = ( 1U << 15 );     // BusFault:    BFAR holds a valid faulting address
    constexpr u32 UNDEFINSTR    = ( 1U << 16 );     // UsageFault:  undefined instruction executed
    constexpr u32 INVSTATE      = ( 1U << 17 );     // UsageFault:  invalid EPSR
    constexpr u32 UNALIGNED     = ( 1U << 24 );     // UsageFault:  unaligned access
    constexpr u32 DIVBYZERO     = ( 1U << 25 );     // UsageFault:  integer divide by zero (needs DIV_0_TRP set)
}

// SCB_AIRCR bit definitions
namespace SCB_AIRCR_bits{
    constexpr u32 VECTKEY       = ( 0x05FAU << 16 );    // Required Key: any write without this is ignored
    constexpr u32 SYSRESETREQ   = ( 1U << 2 );          // Request a full system reset
}


// =================================================================
// NVIC - Nested Vectored Interrupt Controller (ARM Cortex-M3 core peripheral)
// Base address: 0xE000E100
// 
// Enables/Disables, tracks pending state, and prioritizes every peripheral
// IRQ. STM32 implements only the top 4 bits of each 8-bit priority field,
// giving 16 priority levels (0 = highest, 15 = lowest), each stored
// left-justified: 
//                  register value = level << 4. 
// STM32 has 43 IRQs (0-42) - only ISER[0] (IRQ0-31) and the bottom 11
// bits of ISER[1] (IRQ32-42) are ever meaningful, but the array is 
// sized per the full ARMv7 spec (up to 240 IRQs).
// =================================================================
struct NVIC_t {
    std::array<vu32,  8>  ISER;
    std::array<u32,  24>  RESERVED0;
    std::array<vu32,  8>  ICER;
    std::array<u32,  24>  RESERVED1;
    std::array<vu32,  8>  ISPR;
    std::array<u32,  24>  RESERVED2;
    std::array<vu32,  8>  ICPR;
    std::array<u32,  24>  RESERVED3;
    std::array<vu32,  8>  IABR;
    std::array<u32,  56>  RESERVED4;
    std::array<vu8, 240>  IPR;
};

static_assert(sizeof(NVIC_t) == 1008, "NVIC_t size mismatch");

static NVIC_t* const NVIC = reinterpret_cast<NVIC_t*>(0xE000E100U);

// NVIC helper functions
namespace NVIC_helpers {
    // Only the four bits of each byte is used for priority levels
    constexpr u32 PRIO_BITS = 4;

    // Set IRQ 'irq' (0...42) to priority level 'level' (0 = highest... 15= lowest).
    inline void set_priority( int irq, u32 level ){
        NVIC->IPR[irq] = static_cast<u8>((level & 0x0FU ) << 4 );
    }

    // Enable the given irq 'irq'
    inline void enable_irq( int irq ) {
        NVIC->ISER[irq/32] = (1U << ( irq % 32 ));
    }

    // Disable the given irq 'irq'
    inline void disable_irq( int irq ) {
        NVIC->ICER[irq/32] = (1U << ( irq % 32 ));
    }
}



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

static_assert(sizeof(RCC_t)== 40, "RCC_t size mismatch - check register definitions");

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
    vu32 CRL;           // 0x00 Config Low: pins 0-7 (4 bits per pin)
    vu32 CRH;           // 0x04 Config High: pins 8-15 (4 bits per pin)
    vu32 IDR;           // 0x08 Input Data (read-only; reflects pin voltage)
    vu32 ODR;           // 0x0C Output Data (write to drive pins -- NOT ATOMIC!)
    vu32 BSRR;          // 0x10 Bit Set/Reset - ATOMIC pin control:
                        //      Bits [15:0] set pin n HIGH (write 1 to bit n)
                        //      Bits [31:16] reset pin n LOW (write 1 to bit n+16)
                        //      Bits written 0 are ignored.
    vu32 BRR;           // 0x14 Bit reset register (write 1 to bit n -> pin n LOW )
    vu32 LCKR;          // 0x18 Configuration lock
};

static_assert(sizeof(GPIO_t) == 28, "GPIO_t size mismatch" );

static GPIO_t* const GPIOA = reinterpret_cast<GPIO_t*>(0x40010800U);
static GPIO_t* const GPIOC = reinterpret_cast<GPIO_t*>(0x40011000U);


// =================================================================
// FLASH - Flash Programming Interface (FPEC)
// Base Address: 0x40022000
//
// The CPU can READ flash freely, but WRITING requires:
//  1. Unlocking the FPEC with a key sequence (security feature)
//  2. Erasing a full 1KB page (flash can only erase, not overwrite)
//  3. Writing in 16-bit halfwords (the flash array is 16 bits wide)
// =================================================================
struct FLASH_t{
    vu32 ACR;           // 0x00 Access control (flash wait states and latency )
    vu32 KEYR;          // 0x04 Key register - write magic values to unlock
    vu32 OPTKEYR;       // 0x08 Option bytes key register
    vu32 SR;            // 0x0C Status register (BSY,EOP,errors)
    vu32 CR;            // 0x10 Control register (PG,PER,MER,STRT,LOCK)
    vu32 AR;            // 0x14 Address register (for erase operations)
    vu32 RESERVED;
    vu32 OBR;           // 0x1C Option Byte Register (read-only reflection)
    vu32 WRPR;          // 0x20 Write Protection Register
};

static_assert(sizeof(FLASH_t) == 36, "FLASH_t size mismatch" );

static FLASH_t* const FLASH = reinterpret_cast<FLASH_t*>(0x40022000U);


namespace FLASH_bits {
    // ACR - wait states required based on SYSCLK frequency
    // 0-24 MHz: 0 wait states
    // 24-48 MHz: 1 wait state
    // 48-72 MHz: 2 wait states
    constexpr u32 ACR_LATENCY_0WS   = ( 0x0U << 0 );
    constexpr u32 ACR_LATENCY_1WS   = ( 0x1U << 0 );
    constexpr u32 ACR_LATENCY_2WS   = ( 0x2U << 0 );
    constexpr u32 ACR_PRFTBE        = ( 1U << 4 );      // Prefetch buffer enable
    // CR - programming / erase control
    constexpr u32 CR_PG     = ( 1U << 0 );      // Programming enable
    constexpr u32 CR_PER    = ( 1U << 1 );      // Page erase enable
    constexpr u32 CR_MER    = ( 1U << 2 );      // Mass erase enable
    constexpr u32 CR_STRT   = ( 1U << 6 );      // Start erase operation
    constexpr u32 CR_LOCK   = ( 1U << 7 );      // Lock the FPEC (protect writes)
    // SR - Status bits
    constexpr u32 SR_BSY    = ( 1U << 0 );      // Flash busy (operation in progress)
    constexpr u32 SR_EOP    = ( 1U << 5 );      // End of operation flag
    constexpr u32 SR_WRPRTERR = ( 1U << 4 );    // Write protection error
    // Unlock key sequence
    // Writing these exact values to FLASH->KEYR, in order, unlocks the FPEC.
    // Any wrong value, or writing KEYR when already unlocked, triggers a HardFault.
    constexpr u32 KEYR_KEY1 = 0x45670123U;
    constexpr u32 KEYR_KEY2 = 0xCDEF89ABU;
}

// =================================================================
// SysTick - ARM Cortex-M System Timer
// Base Address: 0xE000E010 (System Control Space)
//
// SysTick is part of the ARM Cortex-M3 architecture specification,
// NOT an STM32-specific peripheral. It exists identically at this 
// address on every Cortex-M processor from any silicon vendor.
// 
// It is a 24-bit down-counter:
//  -> Loaded with RELOAD value when started or when it reaches 0
//  -> Counts: RELOAD -> RELOAD - 1 -> .....-> 1 -> 0 -> (fires) -> RELOAD
//  -> Period = (RELOAD + 1) / f_cpu
//  -> For 1ms at 48MHz: (RELOAD + 1 ) = 48,000 -> RELOAD = 47,999
//  -> Maximum RELOAD: 2^24 - 1 = 16,777,215 (24-bit limit)
// =================================================================
struct SysTick_t{
    vu32 CSR;           // 0x00 Control and Status
    vu32 RVR;           // 0x04 Reload Value Register ( 24-bit )
    vu32 CVR;           // 0x08 Current Value Register (write any value to clear to 0)
    vu32 CALIB;         // 0x0C CALIBration (read-only, chip-specific)
};

static_assert(sizeof(SysTick_t) == 16, "SysTick_t size mismatch");

static SysTick_t* const SYSTICK = reinterpret_cast<SysTick_t*>(0xE000E010U);

namespace SysTick_bits {
    constexpr u32 CSR_ENABLE    = ( 1U << 0 );      // 1 = start counting, 0 = stop
    constexpr u32 CSR_TICKINT   = ( 1U << 1 );      // 1 = fire SysTick exception at zero
    constexpr u32 CSR_CLKSOURCE = ( 1U << 2 );      // 1 = CPU Clock (48 MHz), 
                                                    // 0 = CPU Clock / 8 (6 MHz)
    constexpr u32 CSR_COUNTFLAG = ( 1U << 16 );     // Set when counter reaches 0,
                                                    // Cleared automatically on CSR read 
}

/* This is the END ('_') */
