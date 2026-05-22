#include <platform.h>
#include <stdint.h>
#include <gpio.h>

void GPIOSetMode(gpioReg *port, uint8_t pinNum, GPIOmode mode)
{
    
    switch(mode)
    {
        case INPUT_FLOATING_WITHOUTINTERRUPT:   port->DDR &= ~(1u << pinNum);
                                                port->CR1 &= ~(1u << pinNum);
                                                port->CR2 &= ~(1u << pinNum);
                                                break;
        case INPUT_PULLUP_WITHOUTINTERRUPT:     port->DDR &= ~(1u << pinNum);
                                                port->CR1 |= (1u << pinNum);
                                                port->CR2 &= ~(1u << pinNum);
                                                break;
        case INPUT_FLOATING_WITHINTERRUPT:      port->DDR &= ~(1u << pinNum);
                                                port->CR1 &= ~(1u << pinNum);
                                                port->CR2 |= (1u << pinNum);
                                                break;
        case INPUT_PULLUP_WITHINTERRUPT:        port->DDR &= ~(1u << pinNum);
                                                port->CR1 |= (1u << pinNum);
                                                port->CR2 |= (1u << pinNum);
                                                break;
        case OUTPUT_OPEN_DRAIN:                 port->DDR |= (1u << pinNum);
                                                port->CR1 &= ~(1u << pinNum);
                                                port->CR2 &= ~(1u << pinNum);
                                                break;
        case OUTPUT_PUSHPULL:                   port->DDR |= (1u << pinNum);
                                                port->CR1 |= (1u << pinNum);
                                                port->CR2 &= ~(1u << pinNum);
                                                break;
        case OUTPUT_OPEN_DRAIN_FASTMODE:        port->DDR |= (1u << pinNum);
                                                port->CR1 &= ~(1u << pinNum);
                                                port->CR2 |= (1u << pinNum);
                                                break;
        case OUTPUT_PUSHPULL_FASTMODE:          port->DDR |= (1u << pinNum);
                                                port->CR1 |= (1u << pinNum);
                                                port->CR2 |= (1u << pinNum);
                                                break;

    }
    return;

}
GPIOstate GPIOGetValue(gpioReg *port, uint8_t pinNum)
{
    GPIOstate result = INPUT_FLOATING_WITHOUTINTERRUPT;
    /* if pinmode is output */
    if((port->DDR & (1u << pinNum)) == 0x00u)
    {
        result = ((port->ODR)&(1u << pinNum)) >> pinNum;
    }
    else
    {
        result = ((port->IDR)&(1u << pinNum)) >> pinNum;
    }

    return result;
}

void GPIOSetValue(gpioReg *port, uint8_t pinNum, GPIOstate state)
{
    if((port->DDR & (1u << pinNum)) == 0x00u)
    {
        if(state == LOW)
        {
            port->ODR &= ~(1u << pinNum);
        }
        else
        {
            port->ODR |= (1u << pinNum);
        }

    }
}
