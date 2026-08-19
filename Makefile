# =================================================================
# Makefile - STM32F103 Bootloader Build System
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
#	-std=c++20			: Enable C++20 features 
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
#	-fstack-usage		: Outputs a .su file tracking static stack size for every 
#						  function.
# ------------------------------------------------------------------
CXXFLAGS	 = $(ARCH_FLAGS)
CXXFLAGS	+= -std=c++20
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
CXXFLAGS	+= -fstack-usage


# ------------------------------------------------------------------
# Assembly flags
# ------------------------------------------------------------------
ASFLAGS	= $(ARCH_FLAGS)


# ------------------------------------------------------------------
# Linker Flags: 
#	-T linker.ld		: Use our custom linker script (not the default one)
#	--gc-sections		: Remove any section not reachable from the entry point.
#						  This is the garbage collection pass that removes unused
#						  functions and data we compiled but never called.
#						  Works together with -ffunction-sections / -fdata-sections.
#	-nostdlib			: Don't link crtbegin/crtend or libc (no standard startup)
#	-nostartfiles		: Don't use standard startup files (we provide startup.s)
#	--print-memory-usage : Print a summary of flash/SRAM usage after linking
#	-Map				: Generates a complete linker memory map file
# ------------------------------------------------------------------
LDFLAGS		 = -T linker.ld
LDFLAGS		+= -Wl,--gc-sections
LDFLAGS		+= -Wl,--print-memory-usage
LDFLAGS		+= -nostdlib
LDFLAGS		+= -nostartfiles
LDFLAGS		+= -Wl,-Map=$(DEBUGDIR)/$(TARGET).map


TARGET 		= blue_pill_bootloader
SRCDIR		= src
OBJDIR		= build
DEBUGDIR	= $(OBJDIR)/debug


# ------------------------------------------------------------------
# Source files
# ------------------------------------------------------------------
CXX_SRCS	= $(wildcard $(SRCDIR)/*.cpp)
AS_SRCS		= $(SRCDIR)/startup.s


# ------------------------------------------------------------------
# Object files
#	$(Variable: FindPattern = ReplacePattern)
# ------------------------------------------------------------------
CXX_OBJS	= $(CXX_SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
AS_OBJS		= $(AS_SRCS:$(SRCDIR)/%.s=$(OBJDIR)/%.o)
ALL_OBJS	= $(AS_OBJS) $(CXX_OBJS)


# ------------------------------------------------------------------
# Extra debug artifacts
# ------------------------------------------------------------------
DEBUG_LST	= $(CXX_SRCS:$(SRCDIR)/%.cpp=$(DEBUGDIR)/%.lst)
DEBUG_SU	= $(CXX_SRCS:$(SRCDIR)/%.cpp=$(DEBUGDIR)/%.su)
DEBUG_ASM	= $(CXX_SRCS:$(SRCDIR)/%.cpp=$(DEBUGDIR)/%.as)


# ------------------------------------------------------------------
# Default target: build the .bin file
# .elf -> ELF binary (contains debug info, section metadata - for GDB)
# .bin -> raw binary image (stripped, no metadata - what we write to flash)
# .hex -> Intel HEX format (alternative flash format for some tools)
# ------------------------------------------------------------------
all: $(OBJDIR)/$(TARGET).bin debug_artifacts

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(DEBUGDIR):
	@mkdir -p $(DEBUGDIR)


# ------------------------------------------------------------------
# Assemble .s files
# ------------------------------------------------------------------
$(OBJDIR)/%.o: $(SRCDIR)/%.s | $(OBJDIR)
	@echo "  [AS]    $<"
	@$(AS) $(ASFLAGS) -c $< -o $@


# ------------------------------------------------------------------
# Compile .cpp files and generate mixed code listings
# ------------------------------------------------------------------
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) $(DEBUGDIR)
	@echo "  [CXX]    $<"
	@$(CXX) $(CXXFLAGS) -Wa,-adhln=$(DEBUGDIR)/$*.lst -c $< -o $@


# ------------------------------------------------------------------
# Generate clean, annotated assembly files
# ------------------------------------------------------------------
$(DEBUGDIR)/%.as: $(SRCDIR)/%.cpp | $(DEBUGDIR)
	@echo "  [ASM]    Generating assembly -> $@"
	@$(CXX) $(CXXFLAGS) -S -fverbose-asm $< -o $@


# ------------------------------------------------------------------
# Relocate stack usage metrics data (.su) once objects are created
# ------------------------------------------------------------------
$(DEBUGDIR)/%.su: $(OBJDIR)/%.o | $(DEBUGDIR)
	@mv $(SRCDIR)/$*.su $(DEBUGDIR)/$*.su 2>/dev/null || mv $*.su $(DEBUGDIR)/$*.su 2>/dev/null


# ------------------------------------------------------------------
# Link all objects into ELF
# ------------------------------------------------------------------
$(OBJDIR)/$(TARGET).elf: $(ALL_OBJS)
	@echo ""
	@echo "  [LD]    Linking objects -> $@"
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@
	@echo ""
	@echo "  [SIZE]    Section Overview:"
	@$(SIZE) $@
	

# ------------------------------------------------------------------
# Strip ELF to raw binary
#	-O binary 	: output format is raw binary
#	-S			: don't copy relocation info and symbol table
# ------------------------------------------------------------------
$(OBJDIR)/$(TARGET).bin: $(OBJDIR)/$(TARGET).elf
	@echo ""
	@echo "  [OBJCOPY]    Creating raw binary -> $@"
	@$(OBJCOPY) -O binary -S $< $@


# ------------------------------------------------------------------
# Also produce .HEX for tools that prefer it
# ------------------------------------------------------------------
$(OBJDIR)/$(TARGET).hex: $(OBJDIR)/$(TARGET).elf
	@echo "  [OBJCOPY]    Creating HEX image -> $@"
	@$(OBJCOPY) -O ihex $< $@


# ------------------------------------------------------------------
# Debug Collection
# ------------------------------------------------------------------
debug_artifacts: $(DEBUG_LST) $(DEBUG_ASM) $(DEBUG_SU)
	@echo ""
	@echo "**************************************************************"
	@echo "  All debug files successfully generated in /$(DEBUGDIR)"
	@echo "**************************************************************"

# ------------------------------------------------------------------
# Clean
# ------------------------------------------------------------------
clean:
	@echo "  [CLEAN]    Removing build artifacts..."
	@rm -rf $(OBJDIR)
	@mkdir -p $(OBJDIR) $(DEBUGDIR)


# ------------------------------------------------------------------
# FLASH
# ------------------------------------------------------------------
flash: $(OBJDIR)/$(TARGET).bin
	@echo ""
	@echo "  [FLASH]    Flashing target board..."
	@openocd -f interface/cmsis-dap.cfg -f target/stm32f1x.cfg \
	-c "program $(OBJDIR)/$(TARGET).bin verify reset exit 0x08000000"

.PHONY: all clean flash debug_artifacts
	
