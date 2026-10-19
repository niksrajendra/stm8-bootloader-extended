#include <stdint.h>
#include <platform.h>
#include <uart.h>
#include <uartCfg.h>
#include <timer.h>

static UART_TypeDef1 *uart_get(uint8_t channel)
{
#if defined(STM8S_003_H)
    if (channel == 1u)
        return UART1;
#elif defined(STM8S_005_H)
    if (channel == 2u)
        return (UART_TypeDef1 *)UART2;
#elif defined(STM8S_007_H)
    if (channel == 1u)
        return UART1;
    if (channel == 3u)
        return (UART_TypeDef1 *)UART3;
#endif
    return 0;
}

void uart_init(uint32_t baud_rate, modeUART mode, uint8_t channel)
{
    uint32_t f_cpu = 16000000u;
    uint32_t uart_div;
    UART_TypeDef1 *pUART = uart_get(channel);

    if (pUART == 0 || baud_rate == 0u)
        return;

#if defined(STM8S_003_H)
    CLK_PCKENR1 |= 0x08u;
#elif defined(STM8S_005_H)
    CLK_PCKENR1 |= 0x08u;
#elif defined(STM8S_007_H)
    CLK_PCKENR1 |= (channel == 1u) ? 0x04u : 0x08u;
#endif
	
	/* BRR encodes f_cpu / baud_rate as a 12-bit mantissa with 4 fractional bits. */
	uart_div = (f_cpu + (baud_rate / 2u)) / baud_rate;
    uint8_t brr1_val = (uint8_t)((uart_div >> 4) & 0x00FFu);
    uint8_t brr2_val = (uint8_t)((uart_div & 0x000Fu) | ((uart_div >> 8) & 0x00F0u));
    pUART->BRR2 = brr2_val;
    pUART->BRR1 = brr1_val;
    pUART->CR2  = 0x0Cu;
	/* set number of stop bit */
	switch(mode)
	{
        case DATA_9_BIT_STOP_1_BIT: pUART->CR3 &= 0xCFu;
                                    pUART->CR1 |= UART_NINTH_BIT_ENABLE;
            break;
        case DATA_8_BIT_STOP_1_BIT: pUART->CR3 &= 0xCFu;
                                    pUART->CR1 &= ~UART_NINTH_BIT_ENABLE;
            break;
        case DATA_8_BIT_STOP_2_BIT: pUART->CR3 &= 0xEFu;
                                    pUART->CR3 |= 0x20u;
                                    pUART->CR1 &= ~UART_NINTH_BIT_ENABLE;
            break;
        case DATA_8_BIT_STOP_1_5_BIT: pUART->CR3 |= 0x30u;
                                      pUART->CR1 &= ~UART_NINTH_BIT_ENABLE;
            break;
        default: pUART->CR3 &= 0xCFu;
            break;
	}
}

void uart_write(uint8_t data, uint8_t channel)
{
    UART_TypeDef1 *pUART = uart_get(channel);
    if (pUART == 0)
        return;

	pUART->DR = data;
	while (!(pUART->SR & (1u << 6)));
}

uint8_t uart_read(uint8_t channel)
{
    UART_TypeDef1 *pUART = uart_get(channel);
    if (pUART == 0)
        return 0;

    iwdg_refresh();
    while (!(pUART->SR & (1u << 5)));
    return pUART->DR;
}