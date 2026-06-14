#include <platform.h>
#include <gpio.h>
#include <timer.h>
#include <stm8_interrupt_vector.h>

void delay(uint16_t value) {
    for(uint16_t i = 0; i < value; i++) {
        for(uint16_t j = 0; j < 1000; j++);
    }
}

void hse_enable(void) {
    /* Enable HSE crystal oscillator */
    //CLK_ECKRbits.HSEEN = 1u;
    CLK_ECKR |= 0x01;
    while (!(CLK_ECKR & 0x02));
    /* Switch master clock to HSE */
    CLK_SWR = 0xB4;
    while (!(CLK_SWCR & (1 << 3)));
    CLK_SWCR |= (1 << 1);
}

void main() {
    enable_interrupts();
    hse_enable();
    //timer4_init(TIMER1, TIMER_PRESCALER_64, 0xFFFF);
    GPIOSetMode(GPIOC, 3, OUTPUT_PUSHPULL);
    timer4_init();
    while(1) {
        GPIOSetValue(GPIOC, 3, HIGH);
        delay(1000);
        GPIOSetValue(GPIOC, 3, LOW);
        delay(1000);
    }
}