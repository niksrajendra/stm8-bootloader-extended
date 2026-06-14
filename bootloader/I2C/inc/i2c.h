#ifndef I2C_H
#define I2C_H

#include <stdint.h>

typedef enum
{
    I2C_STANDARD_MODE,
    I2C_FAST_MODE
} I2Cmode;

#define I2C_FAST_MODE_ENABLE                0x80u
#define I2C_FAST_MODE_DUTY_CYCLE_16_9       0x40u

void i2c_init(uint32_t clock_speed, I2Cmode mode);
void i2c_send(uint8_t address, uint8_t *data, uint16_t length);
void i2c_deinit(void);

#endif  //I2C_H