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
    
}