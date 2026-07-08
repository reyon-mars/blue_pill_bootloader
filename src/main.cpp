#include "../include/stm32f103.hpp"
#include "../include/system.hpp"

static void delay( vu32 count )
{
    while( count-- ){
        // Empty. The compiler might optimize this away, so make count a volatile.       
    }
}

int main()
{
    clock_init();
    systick_init();

    RCC->APB2ENR |= RCC_APB2ENR_bits::IOPCEN;
    
    constexpr u32 PC13_CONFIG_SHIFT = ( 13 - 8 ) * 4;
    constexpr u32 PC13_CLEAR_MASK   = ~( 0xFU << PC13_CONFIG_SHIFT );
    constexpr u32 PC13_OUTPUT_50MHZ = ( 0b0011 << PC13_CONFIG_SHIFT );
    
    GPIOC->CRH = ( GPIOC->CRH & PC13_CLEAR_MASK ) | PC13_OUTPUT_50MHZ;
    
    constexpr u32 PC13_SET = ( 1U << 13 );
    constexpr u32 PC13_RESET = (1U << ( 16 + 13 ));

    while( true ){
        GPIOC->BSRR = PC13_RESET;
        delay_ms( 500 );
        GPIOC->BSRR = PC13_SET;
        delay_ms(500 );
    }
    return 0;
}