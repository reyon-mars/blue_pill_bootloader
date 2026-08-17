/************************************************************
 *                                                          *
 * stm32f103.hpp                                            *
 *                                                          *
 * Raw register definitions for STM32F103C8T6.              *
 *                                                          *
 * Struct-based peripheral mapping                          *
 *                                                          *
 ************************************************************/

#pragma  once
#include <array>
#include <cstdint>


/*-----------------------------------------------------------*
 *  Type aliases                                             *
 *-----------------------------------------------------------*/
using u8    = uint8_t;
using u16   = uint16_t;
using u32   = uint32_t;

/*-----------------------------------------------------------*
 *  Aliases For Hardware Registers                           *
 *-----------------------------------------------------------*/
using vu8   = volatile uint8_t;     
using vu16  = volatile uint16_t;    
using vu32  = volatile uint32_t;


// ============================================================================
// SCB - System Control Block (ARM Cortex-M3 core peripheral)
// Base address: 0xE000ED00
// 
// This is NOT an ST peripheral, it is  part of the ARMv7-M architecture 
// itself, byte-identical on every Cortex-M3 from any vendor. Governs
// exception behaviour, reports fault causes, and holds VTOR (Vector
// Table Offset Register).
// ============================================================================
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
    vu32 SHCSR;     // 0x24 System Handler Control & State (enable / pending / active bits)
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

// The single SCB peripheral instance. 'inline' means this symbol is 
// not exported to other translation units (avoids link-time collisions 
// when multiple .cpp files include this header).
inline SCB_t* const SCB = reinterpret_cast<SCB_t*>(0xE000ED00U);

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


// ============================================================================
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
// ============================================================================
struct NVIC_t {
    std::array<vu32,  8>  ISER;         // 0x000 Set-Enable
    std::array<u32,  24>  RESERVED0;
    std::array<vu32,  8>  ICER;         // 0x080 Clear-Enable
    std::array<u32,  24>  RESERVED1;
    std::array<vu32,  8>  ISPR;         // 0x100 Set-Pending
    std::array<u32,  24>  RESERVED2;
    std::array<vu32,  8>  ICPR;         // 0x180 Clear-Pending
    std::array<u32,  24>  RESERVED3;
    std::array<vu32,  8>  IABR;         // 0x200 Active Bit (READ-ONLY)
    std::array<u32,  56>  RESERVED4;
    std::array<vu8, 240>  IPR;          // 0x300 Priority (ONE BYTE PER IRQ)
    std::array<u32, 644>  RESERVED5;    // 0x3F0 - 0xDFF
    vu32                  STIR;         // 0xE00 Software Trigger Interrupt Register (WRITE-ONLY)
};

static_assert(sizeof(NVIC_t) == 3588, "NVIC_t size mismatch");

inline NVIC_t* const NVIC = reinterpret_cast<NVIC_t*>(0xE000E100U);

// NVIC helper functions
namespace NVIC_helpers {
    // Only the UPPER 4 bits (MSBs) of each priority byte are used for priority levels
    constexpr u8 PRIO_BITS = 4;
    constexpr u8 NUM_IMPLEMENTED_IRQ = 43;
    
    // Set IRQ 'irq' (0...42) to priority level 'level' (0 = highest... 15= lowest).
    inline void set_priority( int irq, u32 level ){
        NVIC->IPR[irq] = static_cast<u8>(level << (8U - PRIO_BITS));
    }

    // Enable a specific IRQ 'irq' in the NVIC
    inline void enable_irq( int irq ) {
        NVIC->ISER[irq/32] = (1U << ( irq % 32 ));
    }

    // Disable a specific IRQ 'irq' in the NVIC
    inline void disable_irq( int irq ) {
        NVIC->ICER[irq/32] = (1U << ( irq % 32 ));
    }
}


// ============================================================================
// RCC - Reset and Clock Control
// Base address: 0x40021000
//
// Every peripheral on the STM32 is gated behind a clock. The peripheral
// is POWERED OFF by default. The clock for the peripheral must be enabled
// in RCC before the first register write to it, or the write is silently 
// ignored  (the APB bus returns all-ones on reads, writes have no effect).
// ============================================================================
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

static_assert(sizeof(RCC_t) == 40, "RCC_t size mismatch - check register definitions");

inline RCC_t* const RCC = reinterpret_cast<RCC_t*>(0x40021000U);

// RCC_CR bit definitions
namespace RCC_CR_bits {
    constexpr u32 HSION     = ( 1U << 0 );      // Enable internal 8MHz HSI oscillator
    constexpr u32 HSIRDY    = ( 1U << 1 );      // HSI ready flag ( hardware sets this; READ ONLY )
    constexpr u32 HSITRIM   = ( 0x1FU << 3 );   // Manual HSI frequency trim
    constexpr u32 HSEON     = ( 1U << 16 );     // Enable external crystal (HSE)
    constexpr u32 HSERDY    = ( 1U << 17 );     // HSE ready flag ( READ ONLY )
    constexpr u32 HSEBYP    = ( 1U << 18 );     // Bypass HSE oscillator with an external digital clock
    constexpr u32 CSSON     = ( 1U << 19 );     // Clock Security System: Detect HSE failure, auto-fallback to HSI
    constexpr u32 PLLON     = ( 1U << 24 );     // Enable PLL
    constexpr u32 PLLRDY    = ( 1U << 25 );     // PLL locked flag (READ ONLY)
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


// ============================================================================
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
// ============================================================================
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

inline GPIO_t* const GPIOA = reinterpret_cast<GPIO_t*>(0x40010800U);

inline GPIO_t* const GPIOC = reinterpret_cast<GPIO_t*>(0x40011000U);


// ============================================================================
// FLASH - Flash Programming Interface (FPEC)
// Base Address: 0x40022000
//
// The CPU can READ flash freely, but WRITING requires:
//  1. Unlocking the FPEC with a key sequence (security feature)
//  2. Erasing a full 1KB page (flash can only erase, not overwrite)
//  3. Writing in 16-bit halfwords (the flash array is 16 bits wide)
// ============================================================================
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

inline FLASH_t* const FLASH = reinterpret_cast<FLASH_t*>(0x40022000U);

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


// ============================================================================
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
// ============================================================================
struct SysTick_t{
    vu32 CSR;           // 0x00 Control and Status
    vu32 RVR;           // 0x04 Reload Value Register ( 24-bit )
    vu32 CVR;           // 0x08 Current Value Register (write any value to clear to 0)
    vu32 CALIB;         // 0x0C CALIBration (read-only, chip-specific)
};

static_assert(sizeof(SysTick_t) == 16, "SysTick_t size mismatch");

inline SysTick_t* const SYSTICK = reinterpret_cast<SysTick_t*>(0xE000E010U);

namespace SysTick_bits {
    constexpr u32 CSR_ENABLE    = ( 1U << 0 );      // 1 = start counting, 0 = stop
    constexpr u32 CSR_TICKINT   = ( 1U << 1 );      // 1 = fire SysTick exception at zero
    constexpr u32 CSR_CLKSOURCE = ( 1U << 2 );      // 1 = CPU Clock (48 MHz), 
                                                    // 0 = CPU Clock / 8 (6 MHz)
    constexpr u32 CSR_COUNTFLAG = ( 1U << 16 );     // Set when counter reaches 0,
                                                    // Cleared automatically on CSR read 
}


// ============================================================================
// USB - Full-Speed Device Controller (device-only, no OTG/host capability)
// Control/Status registers: 0x40005C00
// Packet Memory Area (PMA): 0x40006000
//
// The PMA is a dedicated 512-byte SRAM inside the USB peripheral, separate 
// from the 20KB main SRAM at 0x20000000, so the CPU's main memory 
// bus cannot reach it except through USB peripheral's APB1 interface.
//
// Every register below, AND every word of PMA, is only 16 BITS WIDE IN SILICON,
// but each one occupies a full 32-bit address slot. This is because the PMA is
// a dual-port SRAM built for 16-bit peripheral access, wired onto a bus that 
// only decodes 32-bit-aligned addresses - so the upper half of every 32-bit
// slot doesn't connect to anything. 
// CONSEQUENCE: We must access every USB register and every PMA word through
// a 'vu16*', never through a 'vu32'.
// ============================================================================
struct USB_t {
    /**********************************************
     * USB_EPnR (USB EndPoint n Register)         *
     * Offset : 0x00 - 0x1F  ( 32 bytes )         *
     *--------------------------------------------*
     * EPnR_t:                                    *
     * Pairs each EPnR with 16-bit padding to     *
     * match the 4-byte hardware stride.          *
     **********************************************/
    struct EPnR_t {
        vu16 value;
        u16  _unused;
    };
    std::array<EPnR_t, 8> EPnR;
    std::array<u16,   16> RESERVED; // 0x20-0x3F
    /**********************************************/
    vu16 CNTR;     u16 _r_cntr;     // 0x40 Control: reset, power-down, suspend/resume, per-event IRQ enables
    vu16 ISTR;     u16 _r_istr;     // 0x44 Interrupt Status: which event fired + which endpoint (EP_ID, DIR)
    vu16 FNR;      u16 _r_fnr;      // 0x48 Frame Number: current USB frame count + error flags from the last SOF
    vu16 DADDR;    u16 _r_daddr;    // 0x4C Device Address: assigned by the host during enumeration + enable bit
    vu16 BTABLE;   u16 _r_btable;   // 0x50 Buffer Table Address: PMA offset where the 8 endpoints' buffer descriptors
                                    // (addr/count pairs) begin.
};

static_assert(sizeof(USB_t::EPnR_t) == 4, "EPnR_t size mismatch");

static_assert(sizeof(USB_t) == 84, "USB_t size mismatch");

inline USB_t* const USB = reinterpret_cast<USB_t*>(0x40005C00U);

// ============================================================================================
// EPnR Bit layout:
//
// Reset Value: 0x0000
//
// ┌────────┬─────────┬─────────┬───────┬─────────┬─────────┬────────┬─────────┬─────────┬─────┐
// |   15   |   14    |  13:12  |  11   |   10:9  |    8    |    7   |   6     |   5:4   | 3:0 |
// ├────────┼─────────┼─────────┼───────┼─────────┼─────────┼────────┼─────────┼─────────┼─────┤
// | CTR_RX | DTOG_RX | STAT_RX | SETUP | EP_TYPE | EP_KIND | CTR_TX | DTOG_TX | STAT_TX |  EA |
// ├────────┼─────────┼─────────┼───────┼─────────┼─────────┼────────┼─────────┼─────────┼─────┤
// | rc_w0  | toggle  | toggle  |  RO   |  R/W    |   R/W   | rc_w0  | toggle  | toggle  | R/W |
// └────────┴─────────┴─────────┴───────┴─────────┴─────────┴────────┴─────────┴─────────┴─────┘
//
//
// Read/Clear on write 0 (rc_w0) fields [CTR_RX, CTR_TX]: 
// hardware SETS these the instant a transfer completes, independent of firmware timing. 
// Software clears one by writing 0; writing 1 leaves it untouched.
//
// toggle fields (DTOG_*, STAT_*): hardware can also flip these on its own.
// Writing 0 leaves a toggle bit untouched, whereas writing 1 flips it.
// ============================================================================================
namespace USB_EPnR_bits {
    // Bits [3:0] EA: Endpoint Address (rw)
    // Assigned by software during initialization (0 to 15). Associates
    // this hardware EPnR register with a logical USB endpoint address.
    constexpr u32 EA_MASK           = ( 0xFU << 0 );
    // Bits [5:4] STAT_TX: Mask for transmit status (toggle)
    //  -> 00 : DISABLED
    //  -> 01 : STALL
    //  -> 10 : NAK
    //  -> 11 : VALID
    constexpr u32 STAT_TX_MASK      = ( 0x3U << 4 );
    constexpr u32 STAT_TX_DISABLED  = ( 0x0U << 4 );
    constexpr u32 STAT_TX_STALL     = ( 0x1U << 4 );
    constexpr u32 STAT_TX_NAK       = ( 0x2U << 4 );
    constexpr u32 STAT_TX_VALID     = ( 0x3U << 4 );
    // Bit 6 DTOG_TX: Transmit data toggle bit (DATA0 <-> DATA1) (toggle)
    // Flips automatically on every successful IN transaction.
    constexpr u32 DTOG_TX           = ( 0x1U << 6 );
    // Bit 7 CTR_TX: Correct Transfer Transmit (IN Token Complete)
    // Hardware sets to 1 when an IN packet is successfully sent to the host.
    constexpr u32 CTR_TX            = ( 0x1U << 7 );

    // Bit 8 EP_KIND: Endpoint Kind / Double Buffer Feature
    // Behaviour depends on EP_TYPE:
    //  -> BULK     : 1 enables Double Buffering mode.
    //  -> CONTROL  : 1 enables Status Out handling (STATUS_OUT)
    constexpr u32 EP_KIND           = ( 0x1U << 8 );
    // Bits [10:9] EP_TYPE: Mask and encoding for USB transfer mechanism
    constexpr u32 EP_TYPE_MASK      = ( 0x3U << 9 );
    constexpr u32 EP_TYPE_BULK      = ( 0x0U << 9 );    // Bulk endpoint
    constexpr u32 EP_TYPE_CONTROL   = ( 0x1U << 9 );    // Control endpoint (Default for EP0)
    constexpr u32 EP_TYPE_ISO       = ( 0x2U << 9 );    // Isochronous endpoint
    constexpr u32 EP_TYPE_INTERRUPT = ( 0x3U << 9 );    // Interrupt endpoint
    // Bit 11 SETUP: Setup Packet Received Flag (ro)
    // Hardware sets to 1 when a valid SETUP packet is received on a 
    // Control endpoint. Read-Only.
    constexpr u32 SETUP             = ( 0x1U << 11 );
    // Bits [13:12] STAT_RX: Mask for receive status (toggle)
    //  -> 00 : DISABLED
    //  -> 01 : STALL
    //  -> 10 : NAK
    //  -> 11 : VALID
    constexpr u32 STAT_RX_MASK      = ( 0x3U << 12 );
    constexpr u32 STAT_RX_DISABLED  = ( 0x0U << 12 );
    constexpr u32 STAT_RX_STALL     = ( 0x1U << 12 );
    constexpr u32 STAT_RX_NAK       = ( 0x2U << 12 );
    constexpr u32 STAT_RX_VALID     = ( 0x3U << 12 );
    // Bit 14 DTOG_RX: Data Toggle for Receive (OUT Direction)
    // Hardware toggles this on every successful OUT transaction (DATA0 <-> DATA1).
    constexpr u32 DTOG_RX           = ( 0x1U << 14 );
    // Bit 15 CTR_RX: Correct Transfer Receive (OUT/SETUP Token Complete)
    // Hardware sets to 1 when an OUT or SETUP packet is successfully received.
    constexpr u32 CTR_RX            = ( 0x1U << 15 );
    // Mask containing all toggle-on-write bits in EPnR.
    constexpr u32 TOGGLE_BITS       = ( DTOG_TX | DTOG_RX | STAT_TX_MASK | STAT_RX_MASK );
}

namespace USB_CNTR_bits {
    // Bits [4:0] - USB Peripheral Control Bits
    // These bits control the operational state. Purely software owned,
    // nothing here is ever changed by the SIE on its own. Ordinary
    // register writes are safe; no invariant-write needed, unlike EPnR.
    constexpr u32 FRES      = ( 1U << 0 );      // Force digital SIE into reset.
    constexpr u32 PDWN      = ( 1U << 1 );      // Power Down ANALOG TRANSCEIVER.
    constexpr u32 LP_MODE   = ( 1U << 2 );      // Enable USB low-power mode.
    constexpr u32 FSUSP     = ( 1U << 3 );      // Force the USB peripheral into suspend.
    constexpr u32 RESUME    = ( 1U << 4 );      // Initiates a USB resume sequence.
    // Bits [15:8] - USB Interrupt Mask Bits
    // Interrupt masks. Setting one of these does not create an
    // underlying event; it only decides whether that event, once it happens,
    // is allowed to escalate into an actual NVIC interrupt. The matching
    // status flag in ISTR still becomes true even if its mask bit here is 0.
    constexpr u32 ESOFM     = ( 1U << 8 );      // Enable interrupt on expected-SOF error.
    constexpr u32 SOFM      = ( 1U << 9 );      // Enable interrupt on Start-of-Frame.
    constexpr u32 RESETM    = ( 1U << 10 );     // Enable interrupt on USB reset detection.
    constexpr u32 SUSPM     = ( 1U << 11 );     // Enable interrupt on USB suspend detection.
    constexpr u32 WKUPM     = ( 1U << 12 );     // Enable interrupt on USB wakeup detection.
    constexpr u32 ERRM      = ( 1U << 13 );     // Enable interrupt on USB error detection.
    constexpr u32 PMAOVRM   = ( 1U << 14 );     // Enable interrupt on PMA overrun detection.
    constexpr u32 CTRM      = ( 1U << 15 );     // Enable interrupt on correct transfer.
}

namespace USB_ISTR_bits {
    // Bits [4:0] - Transaction Metadata (ro) 
    // EP_ID/DIR are metadata attached to CTR specifically; they only
    // mean something in the same instant CTR is set.
    constexpr u32 EP_ID_MASK    = 0xFU;         // Endpoint number associated with the CTR.
    constexpr u32 DIR           = ( 1U << 4 );  // Transfer direction associated with CTR.
    // Bits [14:8] - Bus Event & Interrupt Status Flags (rc_w0)
    // rc_w0: hardware sets these autonomously on the matching bus event;
    // firmware clears one by writing 0 to it; 1 leaves it untouched.
    constexpr u32 ESOF          = ( 1U << 8 );  // Expected Start-of-Frame error detected.
    constexpr u32 SOF           = ( 1U << 9 );  // Start-of-Frame event detected.
    constexpr u32 RESET         = ( 1U << 10 ); // USB Reset sequence detected.
    constexpr u32 SUSP          = ( 1U << 11 ); // USB Suspend condition detected.
    constexpr u32 WKUP          = ( 1U << 12 ); // USB Wakeup event detected.
    constexpr u32 ERR           = ( 1U << 13 ); // USB error detected.
    constexpr u32 PMAOVR        = ( 1U << 14 ); // Packet Memory Area overrun detected.
    // Bit 15 - Correct Transfer Flag
    // Read-Only. Not an independent flag. This bit is the live logical OR
    // of every EPnR[n].CTR_RX / CTR_TX across all 8 endpoints. Cannot
    // be cleared by writing ISTR at all; goes low on its own only once
    // every underlying EPnR completion bit has been cleared.
    constexpr u32 CTR           = ( 1U << 15 ); // At least one endpoint has a correct transfer.
}

namespace USB_DADDR_bits {
    // Bits [6:0] ADD: Device Address
    // 7-bit USB device address assigned by the host during enumeration.
    constexpr u32 ADDR_MASK = 0x7FU;
    // Bit 7 EF: Enable Function
    // When clear (0), the USB macro is disabled and does not respond to any bus
    // activity regardless of ADD's value.
    constexpr u32 EF        = ( 1U << 7 );
}


// ============================================================================
// PMA_Word_t
// 
// Models one CPU-visible address slot of the Packet Memory Area.
// Although the PMA physically consists of 256 x 16-bit words (512 bytes),
// the CPU does not see them packed back-to-back. Instead, each 16-bit
// PMA word is mapped into its own 32-bit address slot, with the 
// lower half-word containing the actual PMA data and the upper half-word
// not implemented in hardware. Consequently, successive PMA words
// are spaced 4 bytes apart in the CPU address space.
// ============================================================================
struct PMA_Word_t {
    vu16 data;
    u16 _padding;
};

static_assert(sizeof(PMA_Word_t) == 4, "PMA_Word_t size mismatch");

using PMA_t = std::array<PMA_Word_t, 256>;

static_assert(sizeof(PMA_t) == 1024, "PMA_t size mismatch");

inline PMA_t& USB_PMA = *reinterpret_cast<PMA_t*>(0x40006000U);


  /*------------------------------------------------------------*/
 /*               This is the END ('~')                        */
/*------------------------------------------------------------*/