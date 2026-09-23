#ifndef STM8_INTERRUPT_VECTOR_H
#define STM8_INTERRUPT_VECTOR_H
#include <platform.h>

extern void adc1_isr(void) __interrupt(ADC1_ISR);
extern void tim4_isr(void) __interrupt(TIM4_ISR);
#if defined(STM8S_003_H) || defined(STM8S_007_H)
extern void uart1_tx_isr(void) __interrupt(UART1_TXC_ISR);
extern void uart1_rx_isr(void) __interrupt(UART1_RXC_ISR);
#if defined(STM8S_007_H)
extern void uart3_tx_isr(void) __interrupt(UART3_TXC_ISR);
extern void uart3_rx_isr(void) __interrupt(UART3_RXC_ISR);
#endif
#endif
#if defined(STM8S_005_H)
extern void uart2_tx_isr(void) __interrupt(UART2_TXC_ISR);
extern void uart2_rx_isr(void) __interrupt(UART2_RXC_ISR);
#endif
extern void dummy_irq29(void) __interrupt(29);

#endif // STM8_INTERRUPT_VECTOR_H