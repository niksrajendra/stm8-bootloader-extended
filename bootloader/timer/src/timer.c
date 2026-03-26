#include <stm8s003.h>
#include <timer.h>

void timer4_init(void)
{
    /* Enable update interrupt */
    TIM4_IER = 0x01u;
    /* enable preload and counter */
    TIM4_CR1 = 0x81u;
    /* prescaler value */
    TIM4_PSCR = 0x07u;
    /* auto reload register */
    TIM4_ARR = 0xFAu;
    TIM4_EGR = 0x01u;
}


void timerUs(uint16_t value)
{
    if(value == 0)
    {

    }
}

void timerMs(uint16_t value)
{
    if(value == 0)
    {

    }
}

void timerS(uint16_t value)
{
    if(value == 0)
    {

    }
}