#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

typedef enum
{
    PORTA,
    PORTB,
    PORTC,
    PORTD,
    PORTE,
    PORTF,
    PORTG,
    PORTH,
    PORTI
}Portname;

typedef enum
{
    INPUT_FLOATING_WITHOUTINTERRUPT,
    INPUT_PULLUP_WITHOUTINTERRUPT,
    INPUT_FLOATING_WITHINTERRUPT,
    INPUT_PULLUP_WITHINTERRUPT,
    OUTPUT_OPEN_DRAIN,
    OUTPUT_PUSHPULL,
    OUTPUT_OPEN_DRAIN_FASTMODE,
    OUTPUT_PUSHPULL_FASTMODE,
}GPIOmode;

typedef enum
{
    LOW,
    HIGH,
    TRISTATE,
}GPIOstate;

typedef struct
{
    volatile uint8_t ODR;
    volatile uint8_t IDR;
    volatile uint8_t DDR;
    volatile uint8_t CR1;
    volatile uint8_t CR2;
}gpioReg;

#define  GPIOA (volatile gpioReg*)0x5000u
#define  GPIOB (volatile gpioReg*)0x5005u
#define  GPIOC (volatile gpioReg*)0x500Au
#define  GPIOD (volatile gpioReg*)0x500Fu
#define  GPIOE (volatile gpioReg*)0x5014u
#define  GPIOF (volatile gpioReg*)0x5019u
#define  GPIOG (volatile gpioReg*)0x501Eu
#define  GPIOH (volatile gpioReg*)0x5023u
#define  GPIOI (volatile gpioReg*)0x5028u


void GPIOSetMode(gpioReg *port, uint8_t pinNum, GPIOmode mode);
GPIOstate GPIOGetValue(gpioReg *port, uint8_t pinNum);
void GPIOSetValue(gpioReg *port, uint8_t pinNum, GPIOstate state);

#endif // GPIO_H
