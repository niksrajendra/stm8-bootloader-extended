#ifndef UART_H
#define UART_H

typedef enum
{
    DATA_8_BIT_STOP_1_BIT,
    DATA_8_BIT_STOP_2_BIT,
    DATA_8_BIT_STOP_1_5_BIT,
    DATA_9_BIT_STOP_1_BIT
}modeUART;

#define UART_NINTH_BIT_RX           0x80u
#define UART_NINTH_BIT_TX           0x40u
#define UART_NINTH_BIT_ENABLE       0x10u

void uart_init(uint32_t baud_rate, modeUART mode, uint8_t channel);
void uart_write(uint8_t data, uint8_t channel);
uint8_t uart_read(uint8_t channel);

#endif
