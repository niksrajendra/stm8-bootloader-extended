#include <stm8s003.h>
#include <../gpio/inc/gpio.h>
#include <spi.h>

void SPI_init(uint32_t clock, SpiMode mode, uint16_t buadrate)
{
    uint16_t ratio;
    #ifdef STM8S_003_H
    GPIOSetMode(GPIOD, 5, OUTPUT_PUSHPULL); // SCK
    GPIOSetMode(GPIOD, 6, OUTPUT_PUSHPULL); // MISO
    GPIOSetMode(GPIOD, 7, OUTPUT_PUSHPULL); // MOSI
    #elif defined(STM8S_005_H)
    GPIOSetMode(GPIOC, 5, OUTPUT_PUSHPULL); // SCK
    GPIOSetMode(GPIOC, 7, OUTPUT_PUSHPULL); // MISO
    GPIOSetMode(GPIOC, 6, OUTPUT_PUSHPULL); // MOSI 
    #endif
    CLK_PCKENR1 |= 0x02u; // Enable SPI clock
    switch(mode)
    {
        case MODE0: SPI_CR1 &= 0xFCu; // CPOL = 0, CPHA = 0
                    break;

        case MODE1: SPI_CR1 &= 0xFDu; // CPOL = 0, CPHA = 1
                    SPI_CR1 |= 0x01u;
                    break;

        case MODE2: SPI_CR1 |= 0x02u; // CPOL = 1, CPHA = 0
                    SPI_CR1 &= 0xFEu;
                    break;

        case MODE3: SPI_CR1 |= 0x03u; // CPOL = 1, CPHA = 1
                    break;
    }
    SPI_CR1 |= 0x40u; // Master mode
    ratio = clock / buadrate;
    if(ratio == 2)
    {
        SPI_CR1 |= 0x00u; // fPCLK/2
    }
    else if(ratio == 4)
    {
        SPI_CR1 |= 0x08u; // fPLCK/4
    }
    else if(ratio == 8)
    {
        SPI_CR1 |= 0x10u; // fPCLK/8
    }
    else if(ratio == 16)
    {
        SPI_CR1 |= 0x18u; // fPCLK/16
    }
    else if(ratio == 32)
    {
        SPI_CR1 |= 0x20u; // fPCLK/32
    }
    else if(ratio == 64)
    {
        SPI_CR1 |= 0x28u; // fPCLK/64
    }
    else if(ratio == 128)
    {
        SPI_CR1 |= 0x30u; // fPCLK/128
    }
    else if(ratio == 256)
    {
        SPI_CR1 |= 0x38u; // fPCLK/256
    }
    // enable SPI peripheral
    SPI_CR1 |= 0x40u;
}

void SPI_write(uint8_t data)
{
    SPI_DR = data;
    while (!(SPI_SR & SPI_SR_TXE));
}

uint8_t SPI_read(void)
{
    while (!(SPI_SR & SPI_SR_RXNE));
    return SPI_DR;
}
