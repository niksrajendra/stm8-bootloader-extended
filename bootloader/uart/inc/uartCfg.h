#ifndef UARTCFG_H
#define UARTCFG_H
#include <platform.h>
#include <uart.h>

typedef struct
{
    volatile uint8_t SR;
    volatile uint8_t DR;
    volatile uint8_t BRR1;
    volatile uint8_t BRR2;
    volatile uint8_t CR1;
    volatile uint8_t CR2;
    volatile uint8_t CR3;
    volatile uint8_t CR4;
    volatile uint8_t CR5;
    volatile uint8_t GTR;
    volatile uint8_t PSCR;
} UART_TypeDef1;

#endif  //UARTCFG_H
