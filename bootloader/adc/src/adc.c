#include <stdint.h>
#include <platform.h>
#include <adc.h>

void adc_init(uint8_t channel)
{
    if(channel == 2)
    {
        	/* Configure ADC channel 2 (PC4)  */
            ADC_CSR |= (1 << 1);
    }
    /* Right-align data */
    ADC_CR2 |= (1 << 3);
    /* Wake ADC from power down */
    ADC_CR1 |= 1 << 0;
}

uint16_t adc_read(void) {
    uint8_t adcH, adcL;
    ADC_CR1 |= (1 << 0);
    while (!(ADC_CSR & (1 << 7)));
    adcL = ADC_DRL;
    adcH = ADC_DRH;
    ADC_CSR &= ~(1 << 7); // Clear EOC flag
    return (adcL | (adcH << 8));
}