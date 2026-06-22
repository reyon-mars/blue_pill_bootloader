# =================================================================
# Makefile - STM324103 Bootloader Build System
#
# Target: arm-none-eabi toolchain (bare metal, no OS, no libc)
# -none-eabi:
#	-> none means no OS (bare metal)
# 	-> eabi means Embedded Application Binary Interface (ARM calling convention)
# =================================================================
PREFIX	= arm-none-eabi-
CC		= $(PREFIX)gcc
CXX		= $(PREFIX)g++
AS		= $(PREFIX)as
OBJCOPY	= $(PREFIX)objcopy
SIZE	= $(PREFIX)size 

# ------------------------------------------------------------------
# Target: STM32F103C8T6 is Cortex-M3
#	-mcpu=cortex-m3		: Generate Cortex-M3 instructions
#	-mthumb				: Emit Thumb-2 instructions (Cortex-M has no ARM mode)
#	-mfloat-abi=soft	: No hardware FPU on Cortex-M3; use software float
# ------------------------------------------------------------------
ARCH_FLAGS = -mcpu=cortex-m3 -mthumb -mfloat-abi=soft

# ------------------------------------------------------------------
# C++ Flags:
#	-std=c++17			: Enable C++17 features 
#	-fno-exceptions		: Disable C++ exceptions (saves ~4KB of unwinding code)
#						  Exceptions require heap allocation and RTTI - both
#						  undesirable in a bare-metal bootloader
#	-fno-rtti			: Disable Run-Time Type Information 
#						  Same reason: no OS, no heap, no need
#	-fno-threadsafe-statics : Don't emit __cxa_guard_acquire/release for local
#							  static initialization.
#	-ffreestanding		: Don't assume hosted environment. Tells GCC we're bare
#						  metal: don't link crt0, don't assume main() returns an
#						  int that the OS reads
#	-nostdlib			: Don't link the standard C library (no printf, no malloc).
#						  We provide our own startup code instead
#	-ffunction-sections : Place each function in its own ELF section. Combined with
#						  --gc-sections in the linker, dead code is eliminated. Without
#						  this, even unused functions would appear in the binary
#	-fdata-sections		: Same idea for data: each variable gets its own section
#	-Wall -Wextra		: Enable all warnings. In embedded systems, a warning is often
#						  a correctness bug, not just a style issue
#	-Og					: Optimise for debugging (-O0 produces unreadable assembly;
#						  -O2/-O3 may inline or reorder things confusingly). For 
#						  release, we will use -Os (optimise for size).
# ------------------------------------------------------------------
CXXFLAGS	 = $(ARCH_FLAGS)
CXXFLAGS	+= -std=c++17
CXXFLAGS	+= -fno-exceptions
CXXFLAGS	+= -fno-rtti
CXXFLAGS	+= -fno-threadsafe-statics
CXXFLAGS	+= -ffreestanding
CXXFLAGS	+= -nostdlib
CXXFLAGS	+= -ffunction-sections
CXXFLAGS	+= -fdata-sections
CXXFLAGS	+= -Wall -Wextra
CXXFLAGS	+= -Og -g
CXXFLAGS	+= -Iinclude

# ------------------------------------------------------------------
# Assembly flags
# ------------------------------------------------------------------
ASFLAGS	= $(ARCH_FLAGS)


# ------------------------------------------------------------------
# Linker Flags: 
#
#------------------------------------------------------------------
