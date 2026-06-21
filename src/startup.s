@ =================================================================
@ startup.s - ARM Cortex-M3 startup for STM32F103 Bootloader
@ =================================================================

@ Unified ARM/Thumb2 syntax; no ambiguous mnemonics
.syntax unified

@ Enable Cortex-M3 specific instructions
.cpu cortex-m3 

@ Emit Thumb2 instructions ( Cortex-M has no ARM mode )
.thumb


@ External symbols defined in linker.ld - referenced here
.extern main
.extern __stack_top
.extern __data_load
.extern __data_start
.extern __data_end
.extern __bss_start
.extern __bss_end
.extern __init_array_start
.extern __init_array_end

@ =================================================================
@ SECTION: .isr_vector - The ARM Cortex-M3 Vector Table
@
@ This section is placed at 0x08000000 by the linker script.
@ The Cortex-M3 hardware reads it on every reset.
@
@ [0] - The first entry of the vector table is not a Handler !
@ The hardware loads this value directly into the MSP register
@ (Main Stack Pointer). It is the initial stack address, not a 
@ function pointer.
@ 
@ [1] - The second entry of the vector table holds the Address
@ of the Reset Handler. The LSB of the address is set to 1 
@ which is the Thumb bit. The CPU jumps to this address after 
@ every power-on/reset.
@ =================================================================


@ "a" = allocated section (takes space in the flash image)
@ "%progbits" = contains data (vs %nobits for .bss)
.section .isr_vector, "a" , %progbits

vector_table:
    
    @ ──────────────────────────────────────────────────────────────
    @ ARM Cortex-M3 Core Exception Vectors ( indices 0-15 )
    @ These are defined by the ARM architecture, not by ST.
    @ ──────────────────────────────────────────────────────────────

    .word __stack_top       @ [0] Initial Main Stack Pointer value ( not a handler! )
    .word Reset_Handler     @ [1] Reset: first code after power-on
    .word NMI_Handler       @ [2] Non-Maskable Interrupt ( cannot be disabled )
    .word HardFault_Handler @ [3] All fault types if no specific handler
    .word MemManage_Handler @ [4] Memory Protection Unit violation
    .word BusFault_Handler  @ [5] Bus Error ( bad memory access address )
    .word UsageFault_Handler@ [6] Undefined instruction, bad alignment, etc.
    .word 0                 @ [7] Reserved by ARM 
    .word 0                 @ [8] Reserved
    .word 0                 @ [9] Reserved
    .word 0                 @ [10] Reserved
    .word SVC_Handler       @ [11] Supervisor Call ( SVC instruction )
    .word DebugMon_Handler  @ [12] Debug Monitor breakpoints
    .word 0                 @ [13] Reserved
    .word PendSV_Handler    @ [14] Pendable service request (RTOS Context Switch)
    .word SysTick_Handler   @ [15] System tick timer

    @ ──────────────────────────────────────────────────────────────
    @ STM32F103 Peripheral Interrupt Vectors (IRQ0-IRQ42)
    @ Index = 16 + IRQ_number.
    @ We only override USB_LP_CAN_RX0 (IRQ20, index 36) in usb.cpp
    @ All others remain as Default_Handler.
    @ ──────────────────────────────────────────────────────────────

    .word WWDG_IRQHandler           @ [16] IRQ0: Window Watchdog
    .word PVD_IRQHandler            @ [17] IRQ1: Power Voltage Detector
    .word TAMPER_IRQHandler         @ [18] IRQ2: Tamper
    .word RTC_IRQHandler            @ [19] IRQ3: RTC
    .word FLASH_IRQHandler          @ [20] IRQ4: Flash controller
    .word RCC_IRQHandler            @ [21] IRQ5: Reset and Clock Control
    .word EXTI0_IRQHandler          @ [22] IRQ6: External interrupt line 0
    .word EXTI1_IRQHandler          @ [23] IRQ7: External interrupt line 1
    .word EXTI2_IRQHandler          @ [24] IRQ8: External interrupt line 2
    .word EXTI3_IRQHandler          @ [25] IRQ9: External interrupt line 3
    .word EXTI4_IRQHandler          @ [26] IRQ10: External interrupt line 4
    .word DMA1_Channel1_IRQHandler  @ [27] IRQ11: DMA1 channel 1
    .word DMA1_Channel2_IRQHandler  @ [28] IRQ12: DMA1 channel 2
    .word DMA1_Channel3_IRQHandler  @ [29] IRQ13: DMA1 channel 3
    .word DMA1_Channel4_IRQHandler  @ [30] IRQ14: DMA1 channel 4
    .word DMA1_Channel5_IRQHandler  @ [31] IRQ15: DMA1 channel 5
    .word DMA1_Channel6_IRQHandler  @ [32] IRQ16: DMA1 channel 6
    .word DMA1_channel7_IRQHandler  @ [33] IRQ17: DMA1 channel 7
    .word ADC1_2_IRQHandler         @ [34] IRQ18: ADC1 and ADC2
    .word USB_HP_CAN_TX_IRQHandler  @ [35] IRQ19: USB High Priority / CAN TX
    .word USB_LP_CAN_RX0_IRQHandler @ [36] IRQ20: USB Low Priority 
    .word CAN_RX1_IRQHandler        @ [37] IRQ21: CAN RX1
    .word CAN_SCE_IRQHandler        @ [38] IRQ22: CAN SCE
    .word EXTI9_5_IRQHandler        @ [39] IRQ23: EXTI lines 5-9
    .word TIM1_BRK_IRQHandler       @ [40] IRQ24: TIM1 break
    .word TIM1_UP_IRQHandler        @ [41] IRQ25: TIM1 update
    .word TIM1_TRG_COM_IRQHandler   @ [42] IRQ26: TIM1 trigger/commutation
    .word TIM1_CC_IRQHandler        @ [43] IRQ27: TIM1 capture/compare
    .word TIM2_IRQHandler           @ [44] IRQ28: TIM2 global
    .word TIM3_IRQHandler           @ [45] IRQ29: TIM3 global
    .word TIM4_IRQHandler           @ [46] IRQ30: TIM4 global
    .word I2C1_EV_IRQHandler        @ [47] IRQ31: I2C1 event
    .word I2C1_ER_IRQHandler        @ [48] IRQ32: I2C1 error
    .word I2C2_EV_IRQHandler        @ [49] IRQ33: I2C2 event
    .word I2C2_ER_IRQHandler        @ [50] IRQ34: I2C2 error
    .word SPI1_IRQHandler           @ [51] IRQ35: SPI1
    .word SPI2_IRQHandler           @ [52] IRQ36: SPI2
    .word USART1_IRQHandler         @ [53] IRQ37: USART1
    .word USART2_IRQHandler         @ [54] IRQ38: USART2
    .word USART3_IRQHandler         @ [55] IRQ39: USART3
    .word EXTI15_10_IRQHandler      @ [56] IRQ40: EXTI lines 10-15
    .word RTCAlarm_IRQHandler       @ [57] IRQ41: RTC alarm via EXTI
    .word USBWakeUp_IRQHandler      @ [58] IRQ42: USB wakeup from suspend

@ =================================================================
@ Reset_Handler - The Very First Code That Runs
@
@ Entered immediately after power-on or reset. At this Point:
@ -> MSP is already loaded from vector_table[0] by hardware
@ -> SRAM contains random garbage ( no .data, no .bss initialized )
@ -> No C++ runtime exists
@ =================================================================


@ "ax" = Tell the assembler that the section is allocatable and executable
.section .text.Reset_Handler, "ax", %progbits

@ Thumb instruction set, force the linker to set the lowest address bit to 1
.thumb_func

@ Define the Reset_Handler lable and make it global (visible to external files)
.global Reset_Handler

@ Registers the lable as an executable function instead of a raw data 
.type   Reset_Handler, %function

Reset_Handler:
    @ ─────────────────────────────────────────────────────────────
    @ STEP 1: Reinitialize the Main Stack Pointer
    @
    @ The hardware already loaded MSP from vector_table[0] before
    @ branching here, which might seem redundant, But a warm reset
    @ (reset via NVIC SYSRESETREQ or watchdog) does NOT reload the MSP from the 
    @ vector table. It jumps to Reset_Handler with whatever SP value was current.
    @ If the stack was deep in the middle of a function call, we would corrupt
    @ our own stack
    @ ─────────────────────────────────────────────────────────────
    ldr     r0, =__stack_top
    msr     msp, r0

    @ ─────────────────────────────────────────────────────────────
    @ STEP 2: Copy .data section from Flash (LMA) to SRAM (VMA)
    @ 
    @ Register assignment:
    @   r0 = source pointer, walks from __data_load toward flash
    @   r1 = destination pointer, walks from __data_start toward __data_end
    @   r2 = one past the last destination byte (__data_end)
    @   r3 = scratch register for the 32-bit word being transferred
    @
    @ We copy 4 bytes ( one word ) at a time. Both regions are word-aligned
    @ (guaranteed by the linker script), so this is safe.
    @
    @ 'ldr r3, [r0], #4' is a post index load:
    @   Load the word AT address r0 into r3, THEN add 4 to r0.
    @   This instruction loads the word and then advances the pointer.
    @ 'str r3, [r1], #4' is the corresponding post-index store.
    @ ─────────────────────────────────────────────────────────────
    ldr     r0, =__data_load    @ r0 -> LMA: initial values in flash
    ldr     r1, =__data_start   @ r1 -> VMA: destination in SRAM
    ldr     r2, =__data_end     @ r2 -> one past last byte to copy

.L_data_copy:
    cmp     r1, r2              @ Is destination pointer >= end ? 
    bhs     .L_data_copy_done   @ If so, we're done ( bhs: branch if higher or same )
    ldr     r3, [r0], #4        @ Load 4 bytes from flash, advance source by 4
    str     r3, [r1], #4        @ Store 4 bytes to SRAM, advance dest by 4
    b       .L_data_copy        @ Branch to the .L_data_copy lable again
.L_data_copy_done:

    @ ─────────────────────────────────────────────────────────────
    @ STEP 3: Zero the .bss section 
    @ The C++ standard ( ISO/IEC 14882, [basic.start.static] ) states
    @ that objects with static storage duration are zero-initialized
    @ before any other initialization. The hardware does NOT do this.
    @ SRAM on power-up/reset contains garbage (essentially random data ).
    @ So, we must zero every byte in .bss manually.
    @ ─────────────────────────────────────────────────────────────
    ldr     r0, =__bss_start    @ r0 -> start of .bss region in SRAM
    ldr     r1, =__bss_end      @ r1 -> one past last byte of .bss
    movs    r2, #0              @ r2 = 0 ( value to store )

.L_bss_zero:
    cmp     r0, r1              @ Have we reached the end ?
    bhs     .L_bss_zero_done    @ If so, done.
    str     r2, [r0], #4        @ Store 0 at current address, advance by 4
    b       .L_bss_zero         @ Start over again
.L_bss_zero_done:

    
    @ ─────────────────────────────────────────────────────────────
    @ STEP 4: Call C++ global constructors
    @ If there are any global C++ objects used, then the compiler generates
    @ the constructors as a regular function and places a pointer to it in 
    @ a special ELF section called .init_array. Our linker script collects all
    @ .init_array sections and exposes __init_array_start / __init_array_end.
    @
    @ We iterate through this array of function pointers and call each one.
    @ 'blx r2' : Branch with link and eXchange, i.e. calls the function at 
    @ address r2, stores the return address in LR (Link Register), and respects 
    @ the Thumb bit in r2.
    @ ─────────────────────────────────────────────────────────────
    ldr     r0, =__init_array_start
    ldr     r1, =__init_array_end
    
.L_call_ctors:
    cmp     r0, r1              @ All constructors called ? 
    bhs     .L_call_ctors_done  @ Done.
    ldr     r2, [r0], #4        @ Load constructor function pointer
    blx     r2                  @ Call it (Thumb-safe indirect call)
    b       .L_call_ctors       @ Start over again.
.L_call_ctors_done:

    @ ─────────────────────────────────────────────────────────────
    @ STEP 5: Call main()
    @ By now, the C++ runtime environment has been full initialized:
    @   -> Stack is set up
    @   -> .data is in SRAM with correct initial values
    @   -> .bss is zeroed
    @   -> Global constructors have all been called
    @
    @ We can now safely call main() as a normal C++ function.
    @ 'bl main' = Branch with Link: jumps to main, stores the return 
    @ address in LR so main() can return.
    @ ─────────────────────────────────────────────────────────────
    bl      main

    @ ─────────────────────────────────────────────────────────────
    @ STEP 6: Catch a returned main()
    @
    @ A correct bootloader's main() never returns. If it does (due 
    @ to a bug), branching into random memory would cause a crash
    @ that is nearly impossible to debug. Instead, we hang here.
    @ A debugger will show the PC stuck at this address, which 
    @ immediately tells us "main() returned unexpectedly".
    @ ─────────────────────────────────────────────────────────────
.L_hang:
    b       .L_hang


    @ ─────────────────────────────────────────────────────────────
    @ The assembler directive .size Reset_Handler, . - Reset_Handler
    @ calculates the total footprint of the function in bytes by 
    @ substracting its starting address from the current location 
    @ pointer (.) at the end of the code block. This metadata is placed
    @ directly into the .elf object file on the PC to help the debugging 
    @ tools map out function boundaries.
    @ ─────────────────────────────────────────────────────────────
.size   Reset_Handler, . - Reset_Handler

@ =================================================================
@ Default_Handler - Catch-All for Unexpected Exceptions/Interrupts
@
@ If any exception fires that we haven't explicitly handled, execution
@ ends up here in an infinite loop. This is intentional: with a debugger
@ connected, we can halt the CPU and inspect the call stack to see
@ exactly which vector fired.
@
@ Without this, an unexpected interrupt would jump to address 0,
@ causing a HardFault, which would itself call this handler
@ =================================================================

.section .text.Default_Handler, "ax", %progbits
.thumb_func
.weak       Default_Handler
.global     Default_Handler
.type       Default_Handler, %function

Default_Handler:
    b       Default_Handler     @ Spin forever - debugger will find us here

.size Default_Handler, . - Default_Handler

@ =================================================================
@ Weak Handler Aliases
@ 
@ Each name below is declared as a weak alias for Default_Handler.
@ "Weak" means: if another source file defines the Handler, that 
@ definition wins. If not, this alias (pointing to Default_Handler)
@ is used.
@ 
@ .thumb_set X, Y is like ".set X, Y" but propagates the Thumb 
@ attribute, ensuring bit 0 is set correctly in the vector table.
@ =================================================================

@ Core Exceptions
.weak       NMI_Handler
.thumb_set  NMI_Handler, Default_Handler

.weak       HardFault_Handler
.thumb_set  HardFault_Handler, Default_Handler

.weak       MemManage_Handler
.thumb_set  MemManage_Handler, Default_Handler

.weak       BusFault_Handler
.thumb_set  BusFault_Handler, Default_Handler

.weak       UsageFault_Handler
.thumb_set  UsageFault_Handler, Default_Handler

.weak       SVC_Handler
.thumb_set  SVC_Handler, Default_Handler

.weak       DebugMon_Handler
.thumb_set  DebugMon_Handler, Default_Handler

.weak       PendSV_Handler
.thumb_set  PendSV_Handler, Default_Handler

.weak       SysTick_Handler
.thumb_set  SysTick_Handler, Default_Handler



