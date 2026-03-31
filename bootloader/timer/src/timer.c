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

/**
 * Initialize watchdog:
 * prescaler = 32, timeout = 63.70ms
 */
void iwdg_init(void) {
    IWDG_KR = IWDG_KEY_ENABLE;
    IWDG_KR = IWDG_KEY_ACCESS;
    IWDG_PR = 2;
    IWDG_KR = IWDG_KEY_REFRESH;
}

/**
 * Kick the dog
 */
void iwdg_refresh() {
    IWDG_KR = IWDG_KEY_REFRESH;
}