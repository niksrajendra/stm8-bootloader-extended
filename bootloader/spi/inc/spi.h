#ifndef SPI_H
#define SPI_H

#include <stdint.h>

typedef enum
{
   MODE0,
   MODE1,
   MODE2,
   MODE3,
}SpiMode;

#define SPI_SR_TXE 0x02u
#define SPI_SR_RXNE 0x01u


void SPI_init(uint32_t clock, SpiMode mode, uint16_t buadrate);
void SPI_write(uint8_t data);
uint8_t SPI_read(void);


#endif // SPI_H
