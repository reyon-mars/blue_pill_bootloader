	.cpu cortex-m3
	.arch armv7-m
	.fpu softvfp
	.eabi_attribute 20, 1	@ Tag_ABI_FP_denormal
	.eabi_attribute 21, 1	@ Tag_ABI_FP_exceptions
	.eabi_attribute 23, 3	@ Tag_ABI_FP_number_model
	.eabi_attribute 24, 1	@ Tag_ABI_align8_needed
	.eabi_attribute 25, 1	@ Tag_ABI_align8_preserved
	.eabi_attribute 26, 1	@ Tag_ABI_enum_size
	.eabi_attribute 30, 1	@ Tag_ABI_optimization_goals
	.eabi_attribute 34, 1	@ Tag_CPU_unaligned_access
	.eabi_attribute 18, 4	@ Tag_ABI_PCS_wchar_t
	.file	"system.cpp"
@ GNU C++17 (15:14.2.rel1-1) version 14.2.1 20241119 (arm-none-eabi)
@	compiled by GNU C version 14.2.0, GMP version 6.3.0, MPFR version 4.2.1, MPC version 1.3.1, isl version isl-0.27-GMP

@ warning: MPFR header version 4.2.1 differs from library version 4.2.2.
@ GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
@ options passed: -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -march=armv7-m -g -Og -std=c++17 -fno-exceptions -fno-rtti -fno-threadsafe-statics -ffreestanding -ffunction-sections -fdata-sections -fstack-usage
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.file 1 "src/system.cpp"
	.section	.text.SysTick_Handler,"ax",%progbits
	.align	1
	.global	SysTick_Handler
	.syntax unified
	.thumb
	.thumb_func
	.type	SysTick_Handler, %function
SysTick_Handler:
.LFB3:
	.loc 1 22 38 view -0
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 23 5 view .LVU1
@ src/system.cpp:23:     g_ms++;
	.loc 1 23 9 is_stmt 0 view .LVU2
	ldr	r2, .L2	@ tmp116,
	ldr	r3, [r2]	@ g_ms.0_1, g_ms
	adds	r3, r3, #1	@ _2, g_ms.0_1,
	str	r3, [r2]	@ _2, g_ms
@ src/system.cpp:24: }
	.loc 1 24 1 view .LVU3
	bx	lr	@
.L3:
	.align	2
.L2:
	.word	_ZN12_GLOBAL__N_14g_msE
	.cfi_endproc
.LFE3:
	.size	SysTick_Handler, .-SysTick_Handler
	.section	.text._Z8delay_msm,"ax",%progbits
	.align	1
	.global	_Z8delay_msm
	.syntax unified
	.thumb
	.thumb_func
	.type	_Z8delay_msm, %function
_Z8delay_msm:
.LVL0:
.LFB5:
	.loc 1 34 22 is_stmt 1 view -0
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 35 5 view .LVU5
@ src/system.cpp:35:     const u32 start = g_ms;
	.loc 1 35 23 is_stmt 0 view .LVU6
	ldr	r3, .L6	@ tmp118,
	ldr	r2, [r3]	@ start, g_ms
.LVL1:
	.loc 1 37 5 is_stmt 1 view .LVU7
.L5:
	.loc 1 37 25 discriminator 1 view .LVU8
@ src/system.cpp:37:     while( (g_ms-start) < ms ){
	.loc 1 37 13 is_stmt 0 discriminator 1 view .LVU9
	ldr	r3, .L6	@ tmp119,
	ldr	r3, [r3]	@ g_ms.2_1, g_ms
@ src/system.cpp:37:     while( (g_ms-start) < ms ){
	.loc 1 37 17 discriminator 1 view .LVU10
	subs	r3, r3, r2	@ _2, g_ms.2_1, start
@ src/system.cpp:37:     while( (g_ms-start) < ms ){
	.loc 1 37 25 discriminator 1 view .LVU11
	cmp	r3, r0	@ _2, ms
	bcc	.L5		@,
@ src/system.cpp:41: }
	.loc 1 41 1 view .LVU12
	bx	lr	@
.L7:
	.align	2
.L6:
	.word	_ZN12_GLOBAL__N_14g_msE
	.cfi_endproc
.LFE5:
	.size	_Z8delay_msm, .-_Z8delay_msm
	.section	.text._Z10clock_initv,"ax",%progbits
	.align	1
	.global	_Z10clock_initv
	.syntax unified
	.thumb
	.thumb_func
	.type	_Z10clock_initv, %function
_Z10clock_initv:
.LFB6:
	.loc 1 56 18 is_stmt 1 view -0
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 75 5 view .LVU14
@ src/system.cpp:75:     FLASH->ACR  =   FLASH_bits::ACR_LATENCY_1WS | FLASH_bits::ACR_PRFTBE;
	.loc 1 75 17 is_stmt 0 view .LVU15
	ldr	r3, .L11	@ tmp122,
	movs	r2, #17	@ tmp123,
	str	r2, [r3]	@ tmp123, MEM[(struct FLASH_t *)1073881088B].ACR
	.loc 1 100 5 is_stmt 1 view .LVU16
@ src/system.cpp:100:     RCC->CFGR = RCC_CFGR_bits::PLLSRC_HSI_DIV2
	.loc 1 100 15 is_stmt 0 view .LVU17
	sub	r3, r3, #4096	@ tmp124, tmp124,
	ldr	r2, .L11+4	@ tmp125,
	str	r2, [r3, #4]	@ tmp125, MEM[(struct RCC_t *)1073876992B].CFGR
	.loc 1 116 5 is_stmt 1 view .LVU18
@ src/system.cpp:116:     RCC->CR |= RCC_CR_bits::PLLON;
	.loc 1 116 10 is_stmt 0 view .LVU19
	ldr	r2, [r3]	@ _1, MEM[(struct RCC_t *)1073876992B].CR
@ src/system.cpp:116:     RCC->CR |= RCC_CR_bits::PLLON;
	.loc 1 116 13 view .LVU20
	orr	r2, r2, #16777216	@ _2, _1,
	str	r2, [r3]	@ _2, MEM[(struct RCC_t *)1073876992B].CR
	.loc 1 131 5 is_stmt 1 view .LVU21
.L9:
	.loc 1 131 12 discriminator 1 view .LVU22
@ src/system.cpp:131:     while( !(RCC->CR & RCC_CR_bits::PLLRDY) ) {}
	.loc 1 131 19 is_stmt 0 discriminator 1 view .LVU23
	ldr	r3, .L11+8	@ tmp128,
	ldr	r3, [r3]	@ _3, MEM[(struct RCC_t *)1073876992B].CR
@ src/system.cpp:131:     while( !(RCC->CR & RCC_CR_bits::PLLRDY) ) {}
	.loc 1 131 12 discriminator 1 view .LVU24
	tst	r3, #33554432	@ _3,
	beq	.L9		@,
	.loc 1 143 5 is_stmt 1 view .LVU25
@ src/system.cpp:143:     RCC->CFGR |= RCC_CFGR_bits::SW_PLL;
	.loc 1 143 10 is_stmt 0 view .LVU26
	ldr	r2, .L11+8	@ tmp129,
	ldr	r3, [r2, #4]	@ _5, MEM[(struct RCC_t *)1073876992B].CFGR
@ src/system.cpp:143:     RCC->CFGR |= RCC_CFGR_bits::SW_PLL;
	.loc 1 143 15 view .LVU27
	orr	r3, r3, #2	@ _6, _5,
	str	r3, [r2, #4]	@ _6, MEM[(struct RCC_t *)1073876992B].CFGR
	.loc 1 151 5 is_stmt 1 view .LVU28
.L10:
	.loc 1 151 50 discriminator 1 view .LVU29
@ src/system.cpp:151:     while( (RCC->CFGR & RCC_CFGR_bits::SWS_MASK) != RCC_CFGR_bits::SWS_PLL ) {}
	.loc 1 151 18 is_stmt 0 discriminator 1 view .LVU30
	ldr	r3, .L11+8	@ tmp131,
	ldr	r3, [r3, #4]	@ _7, MEM[(struct RCC_t *)1073876992B].CFGR
@ src/system.cpp:151:     while( (RCC->CFGR & RCC_CFGR_bits::SWS_MASK) != RCC_CFGR_bits::SWS_PLL ) {}
	.loc 1 151 23 discriminator 1 view .LVU31
	and	r3, r3, #12	@ _8, _7,
@ src/system.cpp:151:     while( (RCC->CFGR & RCC_CFGR_bits::SWS_MASK) != RCC_CFGR_bits::SWS_PLL ) {}
	.loc 1 151 50 discriminator 1 view .LVU32
	cmp	r3, #8	@ _8,
	bne	.L10		@,
@ src/system.cpp:161: }
	.loc 1 161 1 view .LVU33
	bx	lr	@
.L12:
	.align	2
.L11:
	.word	1073881088
	.word	6816768
	.word	1073876992
	.cfi_endproc
.LFE6:
	.size	_Z10clock_initv, .-_Z10clock_initv
	.section	.text._Z12systick_initv,"ax",%progbits
	.align	1
	.global	_Z12systick_initv
	.syntax unified
	.thumb
	.thumb_func
	.type	_Z12systick_initv, %function
_Z12systick_initv:
.LFB7:
	.loc 1 183 20 is_stmt 1 view -0
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 184 5 view .LVU35
@ src/system.cpp:184:     SYSTICK->RVR = 47'999U; // RELOAD: 48,000 ticks x (1/48 MHz) = 1 ms
	.loc 1 184 18 is_stmt 0 view .LVU36
	mov	r3, #-536813568	@ tmp114,
	movw	r2, #47999	@ tmp115,
	str	r2, [r3, #20]	@ tmp115, MEM[(struct SysTick_t *)3758153744B].RVR
	.loc 1 185 5 is_stmt 1 view .LVU37
@ src/system.cpp:185:     SYSTICK->CVR = 0U;      // Clear Current Count and COUNTFLAG before starting
	.loc 1 185 18 is_stmt 0 view .LVU38
	movs	r2, #0	@ tmp117,
	str	r2, [r3, #24]	@ tmp117, MEM[(struct SysTick_t *)3758153744B].CVR
	.loc 1 191 5 is_stmt 1 view .LVU39
@ src/system.cpp:191:     SYSTICK->CSR = SysTick_bits::CSR_ENABLE
	.loc 1 191 18 is_stmt 0 view .LVU40
	movs	r2, #7	@ tmp119,
	str	r2, [r3, #16]	@ tmp119, MEM[(struct SysTick_t *)3758153744B].CSR
@ src/system.cpp:194: }
	.loc 1 194 1 view .LVU41
	bx	lr	@
	.cfi_endproc
.LFE7:
	.size	_Z12systick_initv, .-_Z12systick_initv
	.section	.bss._ZN12_GLOBAL__N_14g_msE,"aw",%nobits
	.align	2
	.type	_ZN12_GLOBAL__N_14g_msE, %object
	.size	_ZN12_GLOBAL__N_14g_msE, 4
_ZN12_GLOBAL__N_14g_msE:
	.space	4
	.text
.Letext0:
	.file 2 "/usr/lib/gcc/arm-none-eabi/14.2.1/include/stdint.h"
	.file 3 "/usr/include/newlib/c++/14.2.1/cstdint"
	.file 4 "src/../include/stm32f103.hpp"
	.file 5 "/usr/include/newlib/c++/14.2.1/arm-none-eabi/thumb/v7-m/nofp/bits/c++config.h"
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x85b
	.2byte	0x5
	.byte	0x1
	.byte	0x4
	.4byte	.Ldebug_abbrev0
	.uleb128 0x11
	.4byte	.LASF117
	.byte	0x21
	.4byte	.LASF118
	.4byte	.LASF119
	.4byte	.LLRL1
	.4byte	0
	.4byte	.Ldebug_line0
	.uleb128 0x3
	.4byte	.LASF0
	.byte	0x2
	.byte	0x22
	.byte	0x17
	.4byte	0x32
	.uleb128 0x8
	.byte	0x1
	.byte	0x6
	.4byte	.LASF2
	.uleb128 0x3
	.4byte	.LASF1
	.byte	0x2
	.byte	0x25
	.byte	0x18
	.4byte	0x45
	.uleb128 0x8
	.byte	0x2
	.byte	0x5
	.4byte	.LASF3
	.uleb128 0x3
	.4byte	.LASF4
	.byte	0x2
	.byte	0x28
	.byte	0x18
	.4byte	0x58
	.uleb128 0x8
	.byte	0x4
	.byte	0x5
	.4byte	.LASF5
	.uleb128 0x3
	.4byte	.LASF6
	.byte	0x2
	.byte	0x2b
	.byte	0x18
	.4byte	0x6b
	.uleb128 0x8
	.byte	0x8
	.byte	0x5
	.4byte	.LASF7
	.uleb128 0x3
	.4byte	.LASF8
	.byte	0x2
	.byte	0x2e
	.byte	0x18
	.4byte	0x7e
	.uleb128 0x8
	.byte	0x1
	.byte	0x8
	.4byte	.LASF9
	.uleb128 0x3
	.4byte	.LASF10
	.byte	0x2
	.byte	0x31
	.byte	0x19
	.4byte	0x91
	.uleb128 0x8
	.byte	0x2
	.byte	0x7
	.4byte	.LASF11
	.uleb128 0x3
	.4byte	.LASF12
	.byte	0x2
	.byte	0x34
	.byte	0x19
	.4byte	0xa9
	.uleb128 0xe
	.4byte	0x98
	.uleb128 0x8
	.byte	0x4
	.byte	0x7
	.4byte	.LASF13
	.uleb128 0x3
	.4byte	.LASF14
	.byte	0x2
	.byte	0x37
	.byte	0x19
	.4byte	0xbc
	.uleb128 0x8
	.byte	0x8
	.byte	0x7
	.4byte	.LASF15
	.uleb128 0x3
	.4byte	.LASF16
	.byte	0x2
	.byte	0x3c
	.byte	0x1d
	.4byte	0x32
	.uleb128 0x3
	.4byte	.LASF17
	.byte	0x2
	.byte	0x3d
	.byte	0x1e
	.4byte	0x45
	.uleb128 0x3
	.4byte	.LASF18
	.byte	0x2
	.byte	0x3e
	.byte	0x1e
	.4byte	0x58
	.uleb128 0x3
	.4byte	.LASF19
	.byte	0x2
	.byte	0x3f
	.byte	0x1e
	.4byte	0x6b
	.uleb128 0x3
	.4byte	.LASF20
	.byte	0x2
	.byte	0x40
	.byte	0x1e
	.4byte	0x7e
	.uleb128 0x3
	.4byte	.LASF21
	.byte	0x2
	.byte	0x41
	.byte	0x1f
	.4byte	0x91
	.uleb128 0x3
	.4byte	.LASF22
	.byte	0x2
	.byte	0x42
	.byte	0x1f
	.4byte	0xa9
	.uleb128 0x3
	.4byte	.LASF23
	.byte	0x2
	.byte	0x43
	.byte	0x1f
	.4byte	0xbc
	.uleb128 0x3
	.4byte	.LASF24
	.byte	0x2
	.byte	0x47
	.byte	0x1c
	.4byte	0x12f
	.uleb128 0x12
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0x3
	.4byte	.LASF25
	.byte	0x2
	.byte	0x48
	.byte	0x1d
	.4byte	0x12f
	.uleb128 0x3
	.4byte	.LASF26
	.byte	0x2
	.byte	0x49
	.byte	0x1d
	.4byte	0x12f
	.uleb128 0x3
	.4byte	.LASF27
	.byte	0x2
	.byte	0x4a
	.byte	0x1d
	.4byte	0x6b
	.uleb128 0x3
	.4byte	.LASF28
	.byte	0x2
	.byte	0x4b
	.byte	0x1d
	.4byte	0x166
	.uleb128 0x8
	.byte	0x4
	.byte	0x7
	.4byte	.LASF29
	.uleb128 0x3
	.4byte	.LASF30
	.byte	0x2
	.byte	0x4c
	.byte	0x1e
	.4byte	0x166
	.uleb128 0x3
	.4byte	.LASF31
	.byte	0x2
	.byte	0x4d
	.byte	0x1e
	.4byte	0x166
	.uleb128 0x3
	.4byte	.LASF32
	.byte	0x2
	.byte	0x4e
	.byte	0x1e
	.4byte	0xbc
	.uleb128 0x3
	.4byte	.LASF33
	.byte	0x2
	.byte	0x53
	.byte	0x19
	.4byte	0x12f
	.uleb128 0x3
	.4byte	.LASF34
	.byte	0x2
	.byte	0x56
	.byte	0x1a
	.4byte	0x166
	.uleb128 0x3
	.4byte	.LASF35
	.byte	0x2
	.byte	0x5b
	.byte	0x19
	.4byte	0x6b
	.uleb128 0x3
	.4byte	.LASF36
	.byte	0x2
	.byte	0x5c
	.byte	0x1a
	.4byte	0xbc
	.uleb128 0x13
	.ascii	"std\000"
	.byte	0x5
	.2byte	0x134
	.byte	0xb
	.4byte	0x277
	.uleb128 0x4
	.byte	0x33
	.4byte	0x26
	.uleb128 0x4
	.byte	0x34
	.4byte	0x39
	.uleb128 0x4
	.byte	0x35
	.4byte	0x4c
	.uleb128 0x4
	.byte	0x36
	.4byte	0x5f
	.uleb128 0x4
	.byte	0x38
	.4byte	0x123
	.uleb128 0x4
	.byte	0x39
	.4byte	0x136
	.uleb128 0x4
	.byte	0x3a
	.4byte	0x142
	.uleb128 0x4
	.byte	0x3b
	.4byte	0x14e
	.uleb128 0x4
	.byte	0x3d
	.4byte	0xc3
	.uleb128 0x4
	.byte	0x3e
	.4byte	0xcf
	.uleb128 0x4
	.byte	0x3f
	.4byte	0xdb
	.uleb128 0x4
	.byte	0x40
	.4byte	0xe7
	.uleb128 0x4
	.byte	0x42
	.4byte	0x1a9
	.uleb128 0x4
	.byte	0x43
	.4byte	0x191
	.uleb128 0x4
	.byte	0x45
	.4byte	0x72
	.uleb128 0x4
	.byte	0x46
	.4byte	0x85
	.uleb128 0x4
	.byte	0x47
	.4byte	0x98
	.uleb128 0x4
	.byte	0x48
	.4byte	0xb0
	.uleb128 0x4
	.byte	0x4a
	.4byte	0x15a
	.uleb128 0x4
	.byte	0x4b
	.4byte	0x16d
	.uleb128 0x4
	.byte	0x4c
	.4byte	0x179
	.uleb128 0x4
	.byte	0x4d
	.4byte	0x185
	.uleb128 0x4
	.byte	0x4f
	.4byte	0xf3
	.uleb128 0x4
	.byte	0x50
	.4byte	0xff
	.uleb128 0x4
	.byte	0x51
	.4byte	0x10b
	.uleb128 0x4
	.byte	0x52
	.4byte	0x117
	.uleb128 0x4
	.byte	0x54
	.4byte	0x1b5
	.uleb128 0x4
	.byte	0x55
	.4byte	0x19d
	.byte	0
	.uleb128 0x14
	.ascii	"u32\000"
	.byte	0x4
	.byte	0x10
	.byte	0x7
	.4byte	0x98
	.uleb128 0xa
	.4byte	0x277
	.uleb128 0xe
	.4byte	0x277
	.uleb128 0x3
	.4byte	.LASF37
	.byte	0x4
	.byte	0x11
	.byte	0x7
	.4byte	0xa4
	.uleb128 0xb
	.4byte	.LASF77
	.byte	0x28
	.byte	0x1d
	.4byte	0x312
	.uleb128 0x5
	.ascii	"CR\000"
	.byte	0x1e
	.4byte	0x28d
	.byte	0
	.uleb128 0x6
	.4byte	.LASF38
	.byte	0x1f
	.4byte	0x28d
	.byte	0x4
	.uleb128 0x5
	.ascii	"CIR\000"
	.byte	0x20
	.4byte	0x28d
	.byte	0x8
	.uleb128 0x6
	.4byte	.LASF39
	.byte	0x21
	.4byte	0x28d
	.byte	0xc
	.uleb128 0x6
	.4byte	.LASF40
	.byte	0x22
	.4byte	0x28d
	.byte	0x10
	.uleb128 0x6
	.4byte	.LASF41
	.byte	0x23
	.4byte	0x28d
	.byte	0x14
	.uleb128 0x6
	.4byte	.LASF42
	.byte	0x24
	.4byte	0x28d
	.byte	0x18
	.uleb128 0x6
	.4byte	.LASF43
	.byte	0x25
	.4byte	0x28d
	.byte	0x1c
	.uleb128 0x6
	.4byte	.LASF44
	.byte	0x26
	.4byte	0x28d
	.byte	0x20
	.uleb128 0x5
	.ascii	"CSR\000"
	.byte	0x27
	.4byte	0x28d
	.byte	0x24
	.byte	0
	.uleb128 0x15
	.ascii	"RCC\000"
	.byte	0x4
	.byte	0x31
	.byte	0x15
	.4byte	0x327
	.4byte	0x40021000
	.uleb128 0xc
	.4byte	0x299
	.uleb128 0xa
	.4byte	0x322
	.uleb128 0x9
	.4byte	.LASF45
	.byte	0x34
	.4byte	0x385
	.uleb128 0x2
	.4byte	.LASF46
	.byte	0x35
	.4byte	0x283
	.byte	0x1
	.uleb128 0x2
	.4byte	.LASF47
	.byte	0x36
	.4byte	0x283
	.byte	0x2
	.uleb128 0x7
	.4byte	.LASF48
	.byte	0x37
	.4byte	0x283
	.4byte	0x10000
	.uleb128 0x7
	.4byte	.LASF49
	.byte	0x38
	.4byte	0x283
	.4byte	0x20000
	.uleb128 0x7
	.4byte	.LASF50
	.byte	0x39
	.4byte	0x283
	.4byte	0x1000000
	.uleb128 0x7
	.4byte	.LASF51
	.byte	0x3a
	.4byte	0x283
	.4byte	0x2000000
	.byte	0
	.uleb128 0x1
	.4byte	0x336
	.uleb128 0x1
	.4byte	0x341
	.uleb128 0x1
	.4byte	0x34c
	.uleb128 0x1
	.4byte	0x35a
	.uleb128 0x1
	.4byte	0x368
	.uleb128 0x1
	.4byte	0x376
	.uleb128 0x9
	.4byte	.LASF52
	.byte	0x3e
	.4byte	0x447
	.uleb128 0x2
	.4byte	.LASF53
	.byte	0x40
	.4byte	0x283
	.byte	0
	.uleb128 0x2
	.4byte	.LASF54
	.byte	0x41
	.4byte	0x283
	.byte	0x1
	.uleb128 0x2
	.4byte	.LASF55
	.byte	0x42
	.4byte	0x283
	.byte	0x2
	.uleb128 0x2
	.4byte	.LASF56
	.byte	0x44
	.4byte	0x283
	.byte	0xc
	.uleb128 0x2
	.4byte	.LASF57
	.byte	0x45
	.4byte	0x283
	.byte	0x8
	.uleb128 0x2
	.4byte	.LASF58
	.byte	0x47
	.4byte	0x283
	.byte	0
	.uleb128 0x2
	.4byte	.LASF59
	.byte	0x49
	.4byte	0x283
	.byte	0
	.uleb128 0xf
	.4byte	.LASF60
	.byte	0x4a
	.4byte	0x283
	.2byte	0x400
	.uleb128 0x2
	.4byte	.LASF61
	.byte	0x4c
	.4byte	0x283
	.byte	0
	.uleb128 0x2
	.4byte	.LASF62
	.byte	0x4e
	.4byte	0x283
	.byte	0
	.uleb128 0x7
	.4byte	.LASF63
	.byte	0x4f
	.4byte	0x283
	.4byte	0x10000
	.uleb128 0x7
	.4byte	.LASF64
	.byte	0x54
	.4byte	0x283
	.4byte	0x280000
	.uleb128 0x7
	.4byte	.LASF65
	.byte	0x57
	.4byte	0x283
	.4byte	0x400000
	.byte	0
	.uleb128 0x1
	.4byte	0x3ad
	.uleb128 0x1
	.4byte	0x3b8
	.uleb128 0x1
	.4byte	0x3c3
	.uleb128 0x1
	.4byte	0x3ce
	.uleb128 0x1
	.4byte	0x3d9
	.uleb128 0x1
	.4byte	0x3e4
	.uleb128 0x1
	.4byte	0x3ef
	.uleb128 0x1
	.4byte	0x3fa
	.uleb128 0x1
	.4byte	0x406
	.uleb128 0x1
	.4byte	0x411
	.uleb128 0x1
	.4byte	0x41c
	.uleb128 0x1
	.4byte	0x42a
	.uleb128 0x1
	.4byte	0x438
	.uleb128 0x9
	.4byte	.LASF66
	.byte	0x5c
	.4byte	0x4d6
	.uleb128 0x2
	.4byte	.LASF67
	.byte	0x5d
	.4byte	0x283
	.byte	0x1
	.uleb128 0x2
	.4byte	.LASF68
	.byte	0x5e
	.4byte	0x283
	.byte	0x4
	.uleb128 0x2
	.4byte	.LASF69
	.byte	0x5f
	.4byte	0x283
	.byte	0x8
	.uleb128 0x2
	.4byte	.LASF70
	.byte	0x60
	.4byte	0x283
	.byte	0x10
	.uleb128 0x2
	.4byte	.LASF71
	.byte	0x61
	.4byte	0x283
	.byte	0x20
	.uleb128 0xf
	.4byte	.LASF72
	.byte	0x62
	.4byte	0x283
	.2byte	0x1000
	.byte	0
	.uleb128 0x1
	.4byte	0x492
	.uleb128 0x1
	.4byte	0x49d
	.uleb128 0x1
	.4byte	0x4a8
	.uleb128 0x1
	.4byte	0x4b3
	.uleb128 0x1
	.4byte	0x4be
	.uleb128 0x1
	.4byte	0x4c9
	.uleb128 0x9
	.4byte	.LASF73
	.byte	0x66
	.4byte	0x526
	.uleb128 0x2
	.4byte	.LASF74
	.byte	0x67
	.4byte	0x283
	.byte	0x1
	.uleb128 0x7
	.4byte	.LASF75
	.byte	0x68
	.4byte	0x283
	.4byte	0x200000
	.uleb128 0x7
	.4byte	.LASF76
	.byte	0x69
	.4byte	0x283
	.4byte	0x800000
	.byte	0
	.uleb128 0x1
	.4byte	0x4fe
	.uleb128 0x1
	.4byte	0x509
	.uleb128 0x1
	.4byte	0x517
	.uleb128 0xb
	.4byte	.LASF78
	.byte	0x1c
	.byte	0x87
	.4byte	0x58e
	.uleb128 0x5
	.ascii	"CRL\000"
	.byte	0x88
	.4byte	0x28d
	.byte	0
	.uleb128 0x5
	.ascii	"CRH\000"
	.byte	0x89
	.4byte	0x28d
	.byte	0x4
	.uleb128 0x5
	.ascii	"IDR\000"
	.byte	0x8a
	.4byte	0x28d
	.byte	0x8
	.uleb128 0x5
	.ascii	"ODR\000"
	.byte	0x8b
	.4byte	0x28d
	.byte	0xc
	.uleb128 0x6
	.4byte	.LASF79
	.byte	0x8c
	.4byte	0x28d
	.byte	0x10
	.uleb128 0x5
	.ascii	"BRR\000"
	.byte	0x90
	.4byte	0x28d
	.byte	0x14
	.uleb128 0x6
	.4byte	.LASF80
	.byte	0x91
	.4byte	0x28d
	.byte	0x18
	.byte	0
	.uleb128 0xd
	.4byte	.LASF81
	.byte	0x96
	.byte	0x16
	.4byte	0x5a2
	.4byte	0x40010800
	.uleb128 0xc
	.4byte	0x535
	.uleb128 0xa
	.4byte	0x59d
	.uleb128 0xd
	.4byte	.LASF82
	.byte	0x97
	.byte	0x16
	.4byte	0x5a2
	.4byte	0x40011000
	.uleb128 0xb
	.4byte	.LASF83
	.byte	0x24
	.byte	0xa3
	.4byte	0x622
	.uleb128 0x5
	.ascii	"ACR\000"
	.byte	0xa4
	.4byte	0x28d
	.byte	0
	.uleb128 0x6
	.4byte	.LASF84
	.byte	0xa5
	.4byte	0x28d
	.byte	0x4
	.uleb128 0x6
	.4byte	.LASF85
	.byte	0xa6
	.4byte	0x28d
	.byte	0x8
	.uleb128 0x5
	.ascii	"SR\000"
	.byte	0xa7
	.4byte	0x28d
	.byte	0xc
	.uleb128 0x5
	.ascii	"CR\000"
	.byte	0xa8
	.4byte	0x28d
	.byte	0x10
	.uleb128 0x5
	.ascii	"AR\000"
	.byte	0xa9
	.4byte	0x28d
	.byte	0x14
	.uleb128 0x6
	.4byte	.LASF86
	.byte	0xaa
	.4byte	0x28d
	.byte	0x18
	.uleb128 0x5
	.ascii	"OBR\000"
	.byte	0xab
	.4byte	0x28d
	.byte	0x1c
	.uleb128 0x6
	.4byte	.LASF87
	.byte	0xac
	.4byte	0x28d
	.byte	0x20
	.byte	0
	.uleb128 0xd
	.4byte	.LASF88
	.byte	0xb1
	.byte	0x17
	.4byte	0x636
	.4byte	0x40022000
	.uleb128 0xc
	.4byte	0x5b6
	.uleb128 0xa
	.4byte	0x631
	.uleb128 0x9
	.4byte	.LASF89
	.byte	0xb4
	.4byte	0x6e6
	.uleb128 0x2
	.4byte	.LASF90
	.byte	0xb9
	.4byte	0x283
	.byte	0
	.uleb128 0x2
	.4byte	.LASF91
	.byte	0xba
	.4byte	0x283
	.byte	0x1
	.uleb128 0x2
	.4byte	.LASF92
	.byte	0xbb
	.4byte	0x283
	.byte	0x2
	.uleb128 0x2
	.4byte	.LASF93
	.byte	0xbc
	.4byte	0x283
	.byte	0x10
	.uleb128 0x2
	.4byte	.LASF94
	.byte	0xbe
	.4byte	0x283
	.byte	0x1
	.uleb128 0x2
	.4byte	.LASF95
	.byte	0xbf
	.4byte	0x283
	.byte	0x2
	.uleb128 0x2
	.4byte	.LASF96
	.byte	0xc0
	.4byte	0x283
	.byte	0x4
	.uleb128 0x2
	.4byte	.LASF97
	.byte	0xc1
	.4byte	0x283
	.byte	0x40
	.uleb128 0x2
	.4byte	.LASF98
	.byte	0xc2
	.4byte	0x283
	.byte	0x80
	.uleb128 0x2
	.4byte	.LASF99
	.byte	0xc4
	.4byte	0x283
	.byte	0x1
	.uleb128 0x2
	.4byte	.LASF100
	.byte	0xc5
	.4byte	0x283
	.byte	0x20
	.uleb128 0x2
	.4byte	.LASF101
	.byte	0xc6
	.4byte	0x283
	.byte	0x10
	.uleb128 0x7
	.4byte	.LASF102
	.byte	0xca
	.4byte	0x283
	.4byte	0x45670123
	.uleb128 0x7
	.4byte	.LASF103
	.byte	0xcb
	.4byte	0x283
	.4byte	0xcdef89ab
	.byte	0
	.uleb128 0x1
	.4byte	0x645
	.uleb128 0x1
	.4byte	0x650
	.uleb128 0x1
	.4byte	0x65b
	.uleb128 0x1
	.4byte	0x666
	.uleb128 0x1
	.4byte	0x671
	.uleb128 0x1
	.4byte	0x67c
	.uleb128 0x1
	.4byte	0x687
	.uleb128 0x1
	.4byte	0x692
	.uleb128 0x1
	.4byte	0x69d
	.uleb128 0x1
	.4byte	0x6a8
	.uleb128 0x1
	.4byte	0x6b3
	.uleb128 0x1
	.4byte	0x6be
	.uleb128 0x1
	.4byte	0x6c9
	.uleb128 0x1
	.4byte	0x6d7
	.uleb128 0xb
	.4byte	.LASF104
	.byte	0x10
	.byte	0xdd
	.4byte	0x764
	.uleb128 0x5
	.ascii	"CSR\000"
	.byte	0xde
	.4byte	0x28d
	.byte	0
	.uleb128 0x5
	.ascii	"RVR\000"
	.byte	0xdf
	.4byte	0x28d
	.byte	0x4
	.uleb128 0x5
	.ascii	"CVR\000"
	.byte	0xe0
	.4byte	0x28d
	.byte	0x8
	.uleb128 0x6
	.4byte	.LASF105
	.byte	0xe1
	.4byte	0x28d
	.byte	0xc
	.byte	0
	.uleb128 0xd
	.4byte	.LASF106
	.byte	0xe6
	.byte	0x19
	.4byte	0x778
	.4byte	0xe000e010
	.uleb128 0xc
	.4byte	0x72c
	.uleb128 0xa
	.4byte	0x773
	.uleb128 0x9
	.4byte	.LASF107
	.byte	0xe8
	.4byte	0x7b7
	.uleb128 0x2
	.4byte	.LASF108
	.byte	0xe9
	.4byte	0x283
	.byte	0x1
	.uleb128 0x2
	.4byte	.LASF109
	.byte	0xea
	.4byte	0x283
	.byte	0x2
	.uleb128 0x2
	.4byte	.LASF110
	.byte	0xeb
	.4byte	0x283
	.byte	0x4
	.uleb128 0x7
	.4byte	.LASF111
	.byte	0xed
	.4byte	0x283
	.4byte	0x10000
	.byte	0
	.uleb128 0x1
	.4byte	0x787
	.uleb128 0x1
	.4byte	0x792
	.uleb128 0x1
	.4byte	0x79d
	.uleb128 0x1
	.4byte	0x7a8
	.uleb128 0x16
	.4byte	0x7dd
	.uleb128 0x17
	.4byte	.LASF112
	.byte	0x1
	.byte	0xb
	.byte	0x12
	.4byte	0x288
	.byte	0
	.uleb128 0x18
	.4byte	0x7d0
	.uleb128 0x5
	.byte	0x3
	.4byte	_ZN12_GLOBAL__N_14g_msE
	.uleb128 0x10
	.4byte	.LASF113
	.byte	0xb7
	.4byte	.LASF115
	.4byte	.LFB7
	.4byte	.LFE7-.LFB7
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x10
	.4byte	.LASF114
	.byte	0x38
	.4byte	.LASF116
	.4byte	.LFB6
	.4byte	.LFE6-.LFB6
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x19
	.4byte	.LASF120
	.byte	0x1
	.byte	0x22
	.byte	0x6
	.4byte	.LASF121
	.4byte	.LFB5
	.4byte	.LFE5-.LFB5
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x84c
	.uleb128 0x1a
	.ascii	"ms\000"
	.byte	0x1
	.byte	0x22
	.byte	0x13
	.4byte	0x277
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x1b
	.4byte	.LASF122
	.byte	0x1
	.byte	0x23
	.byte	0xf
	.4byte	0x283
	.4byte	.LLST0
	.4byte	.LVUS0
	.byte	0
	.uleb128 0x1c
	.4byte	.LASF123
	.byte	0x1
	.byte	0x16
	.byte	0x11
	.4byte	.LFB3
	.4byte	.LFE3-.LFB3
	.uleb128 0x1
	.byte	0x9c
	.byte	0
	.section	.debug_abbrev,"",%progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x34
	.byte	0
	.uleb128 0x47
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1c
	.uleb128 0xb
	.uleb128 0x6c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x8
	.byte	0
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 3
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 11
	.uleb128 0x18
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 10
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 10
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1c
	.uleb128 0x6
	.uleb128 0x6c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x39
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 11
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 8
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1c
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x35
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1c
	.uleb128 0x5
	.uleb128 0x6c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 6
	.uleb128 0x6e
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7a
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x39
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1c
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x39
	.byte	0x1
	.uleb128 0x89
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x34
	.byte	0
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x6e
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7a
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.uleb128 0x2137
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7a
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loclists,"",%progbits
	.4byte	.Ldebug_loc3-.Ldebug_loc2
.Ldebug_loc2:
	.2byte	0x5
	.byte	0x4
	.byte	0
	.4byte	0
.Ldebug_loc0:
.LVUS0:
	.uleb128 .LVU7
	.uleb128 0
.LLST0:
	.byte	0x8
	.4byte	.LVL1
	.uleb128 .LFE5-.LVL1
	.uleb128 0x1
	.byte	0x52
	.byte	0
.Ldebug_loc3:
	.section	.debug_aranges,"",%progbits
	.4byte	0x34
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.LFB3
	.4byte	.LFE3-.LFB3
	.4byte	.LFB5
	.4byte	.LFE5-.LFB5
	.4byte	.LFB6
	.4byte	.LFE6-.LFB6
	.4byte	.LFB7
	.4byte	.LFE7-.LFB7
	.4byte	0
	.4byte	0
	.section	.debug_rnglists,"",%progbits
.Ldebug_ranges0:
	.4byte	.Ldebug_ranges3-.Ldebug_ranges2
.Ldebug_ranges2:
	.2byte	0x5
	.byte	0x4
	.byte	0
	.4byte	0
.LLRL1:
	.byte	0x7
	.4byte	.LFB3
	.uleb128 .LFE3-.LFB3
	.byte	0x7
	.4byte	.LFB5
	.uleb128 .LFE5-.LFB5
	.byte	0x7
	.4byte	.LFB6
	.uleb128 .LFE6-.LFB6
	.byte	0x7
	.4byte	.LFB7
	.uleb128 .LFE7-.LFB7
	.byte	0
.Ldebug_ranges3:
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF88:
	.ascii	"FLASH\000"
.LASF115:
	.ascii	"_Z12systick_initv\000"
.LASF101:
	.ascii	"SR_WRPRTERR\000"
.LASF37:
	.ascii	"vu32\000"
.LASF56:
	.ascii	"SWS_MASK\000"
.LASF55:
	.ascii	"SW_PLL\000"
.LASF40:
	.ascii	"APB1RSTR\000"
.LASF108:
	.ascii	"CSR_ENABLE\000"
.LASF0:
	.ascii	"int8_t\000"
.LASF14:
	.ascii	"uint64_t\000"
.LASF3:
	.ascii	"short int\000"
.LASF78:
	.ascii	"GPIO_t\000"
.LASF64:
	.ascii	"PLLMUL_12\000"
.LASF18:
	.ascii	"int_least32_t\000"
.LASF44:
	.ascii	"BDCR\000"
.LASF109:
	.ascii	"CSR_TICKINT\000"
.LASF27:
	.ascii	"int_fast64_t\000"
.LASF98:
	.ascii	"CR_LOCK\000"
.LASF97:
	.ascii	"CR_STRT\000"
.LASF51:
	.ascii	"PLLRDY\000"
.LASF91:
	.ascii	"ACR_LATENCY_1WS\000"
.LASF112:
	.ascii	"g_ms\000"
.LASF61:
	.ascii	"PPRE2_DIV1\000"
.LASF110:
	.ascii	"CSR_CLKSOURCE\000"
.LASF92:
	.ascii	"ACR_LATENCY_2WS\000"
.LASF66:
	.ascii	"RCC_APB2ENR_bits\000"
.LASF23:
	.ascii	"uint_least64_t\000"
.LASF32:
	.ascii	"uint_fast64_t\000"
.LASF87:
	.ascii	"WRPR\000"
.LASF6:
	.ascii	"int64_t\000"
.LASF38:
	.ascii	"CFGR\000"
.LASF48:
	.ascii	"HSEON\000"
.LASF95:
	.ascii	"CR_PER\000"
.LASF77:
	.ascii	"RCC_t\000"
.LASF8:
	.ascii	"uint8_t\000"
.LASF43:
	.ascii	"APB1ENR\000"
.LASF34:
	.ascii	"uintptr_t\000"
.LASF83:
	.ascii	"FLASH_t\000"
.LASF120:
	.ascii	"delay_ms\000"
.LASF19:
	.ascii	"int_least64_t\000"
.LASF75:
	.ascii	"I2C1EN\000"
.LASF20:
	.ascii	"uint_least8_t\000"
.LASF33:
	.ascii	"intptr_t\000"
.LASF65:
	.ascii	"USBPRE_DIV1\000"
.LASF85:
	.ascii	"OPTKEYR\000"
.LASF7:
	.ascii	"long long int\000"
.LASF90:
	.ascii	"ACR_LATENCY_0WS\000"
.LASF106:
	.ascii	"SYSTICK\000"
.LASF5:
	.ascii	"long int\000"
.LASF60:
	.ascii	"PPRE1_DIV2\000"
.LASF74:
	.ascii	"TIM2EN\000"
.LASF79:
	.ascii	"BSRR\000"
.LASF24:
	.ascii	"int_fast8_t\000"
.LASF39:
	.ascii	"APB2RSTR\000"
.LASF46:
	.ascii	"HSION\000"
.LASF104:
	.ascii	"SysTick_t\000"
.LASF52:
	.ascii	"RCC_CFGR_bits\000"
.LASF41:
	.ascii	"AHBENR\000"
.LASF94:
	.ascii	"CR_PG\000"
.LASF62:
	.ascii	"PLLSRC_HSI_DIV2\000"
.LASF35:
	.ascii	"intmax_t\000"
.LASF76:
	.ascii	"USBEN\000"
.LASF84:
	.ascii	"KEYR\000"
.LASF47:
	.ascii	"HSIRDY\000"
.LASF45:
	.ascii	"RCC_CR_bits\000"
.LASF73:
	.ascii	"RCC_APB1ENR_bits\000"
.LASF117:
	.ascii	"GNU C++17 14.2.1 20241119 -mcpu=cortex-m3 -mthumb -"
	.ascii	"mfloat-abi=soft -march=armv7-m -g -Og -std=c++17 -f"
	.ascii	"no-exceptions -fno-rtti -fno-threadsafe-statics -ff"
	.ascii	"reestanding -ffunction-sections -fdata-sections -fs"
	.ascii	"tack-usage\000"
.LASF9:
	.ascii	"unsigned char\000"
.LASF89:
	.ascii	"FLASH_bits\000"
.LASF42:
	.ascii	"APB2ENR\000"
.LASF59:
	.ascii	"PPRE1_DIV1\000"
.LASF2:
	.ascii	"signed char\000"
.LASF15:
	.ascii	"long long unsigned int\000"
.LASF28:
	.ascii	"uint_fast8_t\000"
.LASF12:
	.ascii	"uint32_t\000"
.LASF100:
	.ascii	"SR_EOP\000"
.LASF29:
	.ascii	"unsigned int\000"
.LASF10:
	.ascii	"uint16_t\000"
.LASF96:
	.ascii	"CR_MER\000"
.LASF81:
	.ascii	"GPIOA\000"
.LASF26:
	.ascii	"int_fast32_t\000"
.LASF82:
	.ascii	"GPIOC\000"
.LASF36:
	.ascii	"uintmax_t\000"
.LASF25:
	.ascii	"int_fast16_t\000"
.LASF113:
	.ascii	"systick_init\000"
.LASF122:
	.ascii	"start\000"
.LASF11:
	.ascii	"short unsigned int\000"
.LASF71:
	.ascii	"IOPDEN\000"
.LASF22:
	.ascii	"uint_least32_t\000"
.LASF31:
	.ascii	"uint_fast32_t\000"
.LASF63:
	.ascii	"PLLSRC_HSE\000"
.LASF4:
	.ascii	"int32_t\000"
.LASF50:
	.ascii	"PLLON\000"
.LASF21:
	.ascii	"uint_least16_t\000"
.LASF30:
	.ascii	"uint_fast16_t\000"
.LASF1:
	.ascii	"int16_t\000"
.LASF107:
	.ascii	"SysTick_bits\000"
.LASF111:
	.ascii	"CSR_COUNTFLAG\000"
.LASF105:
	.ascii	"CALIB\000"
.LASF16:
	.ascii	"int_least8_t\000"
.LASF70:
	.ascii	"IOPCEN\000"
.LASF17:
	.ascii	"int_least16_t\000"
.LASF13:
	.ascii	"long unsigned int\000"
.LASF99:
	.ascii	"SR_BSY\000"
.LASF72:
	.ascii	"SPI1EN\000"
.LASF114:
	.ascii	"clock_init\000"
.LASF121:
	.ascii	"_Z8delay_msm\000"
.LASF123:
	.ascii	"SysTick_Handler\000"
.LASF69:
	.ascii	"IOPBEN\000"
.LASF58:
	.ascii	"HPRE_DIV1\000"
.LASF49:
	.ascii	"HSERDY\000"
.LASF86:
	.ascii	"RESERVED\000"
.LASF57:
	.ascii	"SWS_PLL\000"
.LASF67:
	.ascii	"AFIOEN\000"
.LASF118:
	.ascii	"src/system.cpp\000"
.LASF119:
	.ascii	"/home/mars/c_cpp/stm32/blue_pill_bootloader\000"
.LASF80:
	.ascii	"LCKR\000"
.LASF68:
	.ascii	"IOPAEN\000"
.LASF102:
	.ascii	"KEYR_KEY1\000"
.LASF103:
	.ascii	"KEYR_KEY2\000"
.LASF54:
	.ascii	"SW_HSE\000"
.LASF116:
	.ascii	"_Z10clock_initv\000"
.LASF53:
	.ascii	"SW_HSI\000"
.LASF93:
	.ascii	"ACR_PRFTBE\000"
	.ident	"GCC: (15:14.2.rel1-1) 14.2.1 20241119"
