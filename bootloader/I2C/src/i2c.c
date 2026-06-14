#include <stdint.h>
#include <platform.h>
#include <i2c.h>


void i2c_init(uint32_t clock_speed, I2Cmode mode) 
{
    uint32_t f_cpu = 16000000, ccr_value;
    I2C_CR1 |= (1 << 0); // Enable I2C peripheral
    // Implementation for I2C initialization
    I2C_FREQR = 16; // Assuming a 16 MHz clock, set frequency to 16 MHz 
    if (mode == I2C_STANDARD_MODE)
    {
        I2C_CCRH &= ~I2C_FAST_MODE_ENABLE; // Set the I2C mode to standard
        ccr_value = (f_cpu / (clock_speed * 2)); // Calculate CCR value for standard mode
        I2C_CCRL = ccr_value; // Set the CCR value for standard mode
    }
    else if (mode == I2C_FAST_MODE)
    {
        I2C_CCRH |= I2C_FAST_MODE_ENABLE; // Set the I2C mode to fast
        if(i2C_CCRH & I2C_FAST_MODE_DUTY_CYCLE_16_9)
        {
            ccr_value = ((f_cpu * 9) / (clock_speed * 25)); // Calculate CCR value for fast mode with duty cycle 16/9
        }
        else
        {
            ccr_value = (f_cpu / (clock_speed * 3)); // Calculate CCR value for fast mode with duty cycle 2 
        }
        //ccr_value = (f_cpu / (clock_speed * 3)); // Calculate CCR value for fast mode
        I2C_CCRL = ccr_value; // Set the CCR value for fast mode
    }
    
}

void i2c_send(uint8_t address, uint8_t *data, uint16_t length) 
{
    // Implementation for sending data over I2C
    I2C_DR = address;
}

void i2c_deinit() 
{
    I2C_CR1 &= ~(1 << 0); // Disable I2C peripheral
}