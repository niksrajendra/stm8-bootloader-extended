#include <stdint.h>
#include <platform.h>
#include <uart.h>
#include <timer.h>

void uart_init(uint32_t baud_rate, modeUART mode, uint8_t channel)
{
	uint32_t f_cpu = 16000000,uart_div;
    UART_TypeDef1 *pUART;
    if(channel == 1)
    {
        /* enable UART1 clock */
        CLK_PCKENR1 |= 0x80u;
        pUART = UART1;
    }
    if(channel == 2)
    {
        /* enable UART2 clock */
        CLK_PCKENR1 |= 0x40u;
        //pUART = UART2;
    }
	
	/* check divisor of baud rate for least error */
	uart_div = (f_cpu + (baud_rate/2) + 1u)/baud_rate;
	//UART1_BRR2 = 0x00;
	//UART1_BRR2 = ((uart_div & 0xF000u)>>8 | (uart_div & 0x000Fu));
    pUART->BRR2 = ((uart_div & 0xF000u)>>8 | (uart_div & 0x000Fu));
	//UART1_BRR1 = 0x0D;
	//UART1_BRR1 = (uart_div & 0x0FF0u)>>4;
    pUART->BRR1 = (uart_div & 0x0FF0u)>>4;
	//UART1_CR2  = 0x0Cu;
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

	//UART1_CR3 = 0x00
}

void uart_write(uint8_t data, uint8_t channel)
{
    UART_TypeDef1 *pUART = UART1;
    if(channel == 1)
    {
        
    }
	pUART->DR = data;
	while (!(pUART->SR & (1 << 6)));
}

uint8_t uart_read(uint8_t channel)
{
    UART_TypeDef1 *pUART = UART1;
    iwdg_refresh();
    while (!(pUART->SR & (1 << 5)));
    return pUART->DR;
}