#ifndef STM8_INTERRUPT_VECTOR_H
#define STM8_INTERRUPT_VECTOR_H
#include <platform.h>

extern void adc1_isr(void) __interrupt(ADC1_ISR);
extern void tim4_isr(void) __interrupt(TIM4_ISR);
extern void uart_tx_isr(void) __interrupt(UART1_TXC_ISR);
extern void uart_rx_isr(void) __interrupt(UART1_RXC_ISR);
extern void dummy_irq29(void) __interrupt(29);

#endif // STM8_INTERRUPT_VECTOR_H