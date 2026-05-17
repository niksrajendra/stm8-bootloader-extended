#include <stdint.h>
#include <platform.h>
#include <uart.h>

void uart_init(uint32_t baud_rate, modeUART mode)
{
	uint32_t f_cpu = 16000000,uart_div;
	if(mode == DATA_9_BIT_STOP_1_BIT)
    {
        UART1_CR1 |= 0x10u;
    }
    else
    {
        UART1_CR1 &= 0xEFu;
    }
	/* check divisor of baud rate for least error */
	uart_div = (f_cpu + (baud_rate/2) + 1u)/baud_rate;
	//UART1_BRR2 = 0x00;
	UART1_BRR2 = ((uart_div & 0xF000u)>>8 | (uart_div & 0x000Fu));
	//UART1_BRR1 = 0x0D;
	UART1_BRR1 = (uart_div & 0x0FF0u)>>4;
	UART1_CR2  = 0x0Cu;
	/* set number of stop bit */
	switch(mode)
	{
        case DATA_8_BIT_STOP_1_BIT: UART1_CR3 &= 0xCFu;
            break;
        case DATA_8_BIT_STOP_2_BIT: UART1_CR3 &= 0xEFu;
                                    UART1_CR3 |= 0x20u;
            break;
        case DATA_8_BIT_STOP_1_5_BIT: UART1_CR3 |= 0x30u;
            break;
        default: UART1_CR3 &= 0xCFu;
            break;
	}

	//UART1_CR3 = 0x00
}

void uart_write(uint8_t data, uint8_t channel)
{
	UART1_DR = data;
	while (!(UART1_SR & (1 << 6)));
}

uint8_t uart_read(uint8_t channel)
{
    while (!(UART1_SR & (1 << 5)));
    return UART1_DR;
}