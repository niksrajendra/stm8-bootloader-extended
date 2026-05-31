#include <platform.h>
#include <gpio.h>
#include <timer.h>
#include <stm8_interrupt_vector.h>

void delay(uint16_t value) {
    for(uint16_t i = 0; i < value; i++) {
        for(uint16_t j = 0; j < 1000; j++);
    }
}

void main() {
    enable_interrupts();
    //timer4_init(TIMER1, TIMER_PRESCALER_64, 0xFFFF);
    GPIOSetMode(GPIOC, OUTPUT_PUSHPULL_FASTMODE, 3);
    timer4_init();
    while(1) {
        //GPIOSetValue(GPIOC, 3, 1);
        //delay(500);
        //GPIOSetValue(GPIOC, 3, 0);
        //delay(500);
    }
}