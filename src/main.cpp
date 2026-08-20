#include "../include/stm32f103.hpp"
#include "../include/system.hpp"
#include "../include/usb.hpp"



int main() {
    clock_init();
    systick_init();
    usb_init();

    RCC->APB2ENR |= RCC_APB2ENR_bits::IOPCEN;
    constexpr u32 PIN13_SHIFT = (13 - 8) * 4;
    GPIOC->CRH = (GPIOC->CRH & ~(0xFU << PIN13_SHIFT)) | (0x3U << PIN13_SHIFT);

    constexpr u32 PC13_LOW  = (1U << (13 + 16));  // LED on
    constexpr u32 PC13_HIGH = (1U << 13);          // LED off

    while (true) {
        if (g_usb_setup_seen) {
            GPIOC->BSRR = PC13_LOW;      // solid ON: first SETUP arrived
        } else {
            GPIOC->BSRR = PC13_LOW;  delay_ms(100);
            GPIOC->BSRR = PC13_HIGH; delay_ms(100);  // fast blink: still waiting
        }
    }
}