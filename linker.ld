MEMORY
{
    /* Only the first 8KB of flash belongs to us.
        The linker will error if we try to exceed this limit. */
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 8K

    /* Entire SRAM ( 20 KB ) */
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

/*
 * __stack_top : the initial value of the Main Stack Pointer.
 *
 * The Cortex-M3 reads this from the vector_table[0] before executing
 * Reset_Handler. We Place the stack at the very top of the SRAM because
 * the stack grows downwards, starting at the top gives us the maximim room.
 *
 * __stack_top = 0x20000000 + 20 * 1024 = 0x20005000
 *
 */

__stack_top = ORIGIN(SRAM) + LENGTH(SRAM);

/*
 * OUTPUT SECTIONS
 * Each output section collects specific input sections from all 
 * compiled object files and places them into a memory region.
 */

SECTIONS
{
    /* ─────────────────────────────────────────────────────────────
     * .isr_vector - ARM Cortex-M3 Vector Table
     * MUST be the very first thing at 0x08000000
     *
     * Place the vector table at the start of the Flash and keep all 
     * sections even if there is no explicit references to it.
     *
      ───────────────────────────────────────────────────────────── */
    .isr_vector :
    {
        KEEP(*(.isr_vector))
    } > FLASH


    /* ─────────────────────────────────────────────────────────────
     * .text - Executable code and read-only data
     *  Both VMA and the LMA are in FLASH ( code runs directly from flash ).
     */
    .text :
    {
        *(.text)        /* All regular code sections */
        *(.text.*)      /* Per-function sections ( from -ffunction-sections ) */
        *(.rodata)      /* All Read-only Data : string literals, const globals */
        *(.rodata.*)

        /* 
         * C++ global constructor/destructor pointer arrays.
         * Creating a global object of any class, results in the compiler
         * generating a constructor function, which it places in the 
         * .init_array. Our startup code traverses this array and calls 
         * each constructor before calling main().
         */
        __preinit_array_start = .;
        KEEP(*(.preinit_array*))
        __preinit_array_end = .;

        __init_array_start = .;
        KEEP(*(.init_array*))
        __init_array_end = .;

        __fini_array_start = .;
        KEEP(*(.fini_array*))
        __fini_array_end = .;
    } > FLASH

    /*
     * Record where .data's initial values live in flash.
     * LOADADDR(.data) evalutes to the LMA of the .data section 
     * i.e., where in flash the initial values are stored.
     * startup.s read __data_load as the source for the SRAM copy
     * and places it in the SRAM addresses (VMA).
     * 
     * This assignment must come AFTER .text because .data's LMA
     * follows immediately after .text in the flash image.
     */
    __data_load = LOADADDR(.data);

    /* ─────────────────────────────────────────────────────────────
     * .data - Initialized global and staic variables
     * VMA: SRAM ( where .data live at runtime - writable )
     * LMA: FLASH ( where .data initial values are stored - persistent )
     * ">SRAM AT>FLASH" means:
     * Assign VMA addresses from SRAM ( variables live here )
     * Place LMA in FLASH after the previous FLASH section ( initial values here )
      ───────────────────────────────────────────────────────────── */
    .data :
    {
        __data_start = .;  /* Start of .data in SRAM (VMA) */
        *(.data)
        *(.data.*)
        __data_end = .;    /* End of .data in SRAM (VMA) */
    } >SRAM AT>FLASH

    /* ─────────────────────────────────────────────────────────────
     * .bss - Zero-initialized global and static variables
     * 
     * Only a VMA exists (in SRAM). There are no initial values to
     * store in flash because they are all zero by C++ standard.
     * startup.s zeros this entire region before calling main().
     ───────────────────────────────────────────────────────────── */
    .bss :
    {
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)   /* C-style "common" uninitialized global */
        __bss_end = .;
    } >SRAM

    /* ─────────────────────────────────────────────────────────────
     * Discard C++ exception handling tables.
     * We compile with -fno-exceptions, so these are generated but
     * empty. Discarding them keeps the flash image lean.
     ───────────────────────────────────────────────────────────── */
    /DISCARD/ :
    {
        *(.ARM.exidx*)
        *(.ARM.extab*)
        *(.eh_frame*)
    }
}