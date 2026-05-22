#include<platform.h>
#include<gpio.h>
#include<timer.h>


void main() {
    gpio_init(GPIOC, GPIO_MODE_OUT_PP_HIGH_SLOW, GPIO_PIN_3);
    timer_init(TIMER1, TIMER_PRESCALER_64, 0xFFFF);
    while(1) {
        GPIOSetValue(GPIOC, GPIO_PIN_3, 1);
        timer_delay_ms(TIMER1, 500);
        GPIOSetValue(GPIOC, GPIO_PIN_3, 0);
        timer_delay_ms(TIMER1, 500);
    }
}