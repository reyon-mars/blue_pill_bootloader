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
	.file	"dfu.cpp"
@ GNU C++17 (15:14.2.rel1-1) version 14.2.1 20241119 (arm-none-eabi)
@	compiled by GNU C version 14.2.0, GMP version 6.3.0, MPFR version 4.2.1, MPC version 1.3.1, isl version isl-0.27-GMP

@ warning: MPFR header version 4.2.1 differs from library version 4.2.2.
@ GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
@ options passed: -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -march=armv7-m -g -Og -std=c++17 -fno-exceptions -fno-rtti -fno-threadsafe-statics -ffreestanding -ffunction-sections -fdata-sections -fstack-usage
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.file 1 "src/dfu.cpp"
.Letext0:
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF0:
	.ascii	"GNU C++17 14.2.1 20241119 -mcpu=cortex-m3 -mthumb -"
	.ascii	"mfloat-abi=soft -march=armv7-m -g -Og -std=c++17 -f"
	.ascii	"no-exceptions -fno-rtti -fno-threadsafe-statics -ff"
	.ascii	"reestanding -ffunction-sections -fdata-sections -fs"
	.ascii	"tack-usage\000"
.LASF1:
	.ascii	"/home/mars/c_cpp/stm32/blue_pill_bootloader\000"
.LASF2:
	.ascii	"src/dfu.cpp\000"
	.ident	"GCC: (15:14.2.rel1-1) 14.2.1 20241119"
