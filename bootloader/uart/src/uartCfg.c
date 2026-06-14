#include <stdint.h>
#include <platform.h>
#include <uart.h>
#include <timer.h>

typedef struct
{
    #if defined(STM8S_003_H) || defined(STM8S_007_H)
    UART1                    ((UART_TypeDef1 *) 0x5230);
    #endif

    #if defined(STM8S_005_H) || defined(STM8S_007_H)
    UART2                    ((UART_TypeDef2 *) 0x5240);
    #endif
    
}UartConfig;

// UART_TypeDef1 *UART1 = (UART_TypeDef1 *) 0x5230;
// UART_TypeDef2 *UART2 = (UART_TypeDef2 *) 0x5240;
