#ifndef STM8_INTERRUPT_VECTOR_H
#define STM8_INTERRUPT_VECTOR_H
#include <stm8s003.h>

extern void adc1_isr(void) __interrupt(ADC1_ISR);
extern void tim4_isr(void) __interrupt(TIM4_ISR);
extern void uart_isr(void) __interrupt(UART1_TXC_ISR);

#endif // STM8_INTERRUPT_VECTOR_H