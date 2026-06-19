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
    .word 
