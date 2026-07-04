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
    volatile uint8_t CR6;
    volatile uint8_t GTR;
    volatile uint8_t PSCR;
} UART_TypeDef2;

#if defined(STM8S_003_H) || defined(STM8S_007_H)
#define UART1                    ((UART_TypeDef1 *) 0x5230)
#endif

#if defined(STM8S_005_H)
#define UART2                    ((UART_TypeDef2 *) 0x5240)
#endif

#if defined(STM8S_007_H)
#define UART3                    ((UART_TypeDef2 *) 0x5240)
#endif

#endif  //UARTCFG_H
