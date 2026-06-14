#ifndef STM8S_003_H
#define STM8S_003_H

#include <stdint.h>

#define _SFR_(mem_addr)             (*(volatile uint8_t *)(0x5000 + (mem_addr)))
#define _SFR16_(mem_addr)           (*(volatile uint16_t *)(0x5000 + (mem_addr)))  
#define _CFR_(mem_addr)				(*(volatile uint8_t *)(0x7F00 + (mem_addr)))
#define _OPTION_BYTE_(mem_addr)		(*(volatile uint8_t *)(0x4800 + (mem_addr)))

/* PORT A */
#define PA_ODR			            _SFR_(0x00)
#define PA_IDR			            _SFR_(0x01)
#define PA_DDR			            _SFR_(0x02)
#define PA_CR1			            _SFR_(0x03)
#define PA_CR2			            _SFR_(0x04)

/* PORT B */
#define PB_ODR			            _SFR_(0x05)
#define PB_IDR			            _SFR_(0x06)
#define PB_DDR			            _SFR_(0x07)
#define PB_CR1			            _SFR_(0x08)
#define PB_CR2			            _SFR_(0x09)

/* PORT C */
#define PC_ODR			            _SFR_(0x0A)
#define PC_IDR			            _SFR_(0x0B)
#define PC_DDR			            _SFR_(0x0C)
#define PC_CR1			            _SFR_(0x0D)
#define PC_CR2			            _SFR_(0x0E)

/* PORT D */
#define PD_ODR			            _SFR_(0x0F)
#define PD_IDR			            _SFR_(0x10)
#define PD_DDR			            _SFR_(0x11)
#define PD_CR1			            _SFR_(0x12)
#define PD_CR2			            _SFR_(0x13)

/* PORT E */
#define PE_ODR			            _SFR_(0x14)
#define PE_IDR			            _SFR_(0x15)
#define PE_DDR			            _SFR_(0x16)
#define PE_CR1			            _SFR_(0x17)
#define PE_CR2			            _SFR_(0x18)

/* PORT F */
#define PF_ODR			            _SFR_(0x19)
#define PF_IDR			            _SFR_(0x1A)
#define PF_DDR			            _SFR_(0x1B)
#define PF_CR1			            _SFR_(0x1C)
#define PF_CR2			            _SFR_(0x1D)

/* FLASH */
#define FLASH_CR1		            _SFR_(0x5A)
#define FLASH_CR2		            _SFR_(0x5B)
#define FLASH_NCR2		            _SFR_(0x5C)
#define FLASH_FPR		            _SFR_(0x5D)
#define FLASH_NFPR		            _SFR_(0x5E)
#define FLASH_IAPSR		            _SFR_(0x5F)
#define FLASH_PUKR		            _SFR_(0x62)
#define FLASH_DUKR		            _SFR_(0x64)

/* ITC */
#define EXTI_CR1		            _SFR_(0xA0)
#define EXTI_CR2		            _SFR_(0xA1)

/* RST */
#define RST_SR			            _SFR_(0xB3)

/* CLK */
#define CLK_ICKR		            _SFR_(0xC0)
#define CLK_ECKR		            _SFR_(0xC1)
#define CLK_CMSR		            _SFR_(0xC3)
#define CLK_SWR			            _SFR_(0xC4)
#define CLK_SWCR		            _SFR_(0xC5)
#define CLK_CKDIVR		            _SFR_(0xC6)
#define CLK_PCKENR1		            _SFR_(0xC7)
#define CLK_CSSR		            _SFR_(0xC8)
#define CLK_CCOR		            _SFR_(0xC9)
#define CLK_PCKENR2		            _SFR_(0xCA)
#define CLK_HSITRIMR	            _SFR_(0xCC)
#define CLK_SWIMCCR		            _SFR_(0xCD)

/* WWDG */
#define WWDG_CR			            _SFR_(0xD1)
#define WWDG_WR			            _SFR_(0xD2)

/* IWDG */
#define IWDG_KR			            _SFR_(0xE0)
#define IWDG_PR			            _SFR_(0xE1)
#define IWDG_RLR		            _SFR_(0xE2)

/* AWU */
#define AWU_CSR1		            _SFR_(0xF0)
#define AMU_APR			            _SFR_(0xF1)
#define AMU_TBR			            _SFR_(0xF2)

/* BEEP */
#define BEEP_CSR		            _SFR_(0xF3)

/* SPI */
#define SPI_CR1			            _SFR_(0x200)
#define SPI_CR2			            _SFR_(0x201)
#define SPI_ICR			            _SFR_(0x202)
#define SPI_SR			            _SFR_(0x203)
#define SPI_DR			            _SFR_(0x204)
#define SPI_CRCPR		            _SFR_(0x205)
#define SPI_RXCRCR		            _SFR_(0x206)
#define SPI_TXCRCR		            _SFR_(0x207)

/* I2C */
#define I2C_CR1			            _SFR_(0x210)
#define I2C_CR2			            _SFR_(0x211)
#define I2C_FREQR		            _SFR_(0x212)
#define I2C_OARL		            _SFR_(0x213)
#define I2C_OARH		            _SFR_(0x214)
#define I2C_DR			            _SFR_(0x216)
#define I2C_SR1			            _SFR_(0x217)
#define I2C_SR2			            _SFR_(0x218)
#define I2C_SR3			            _SFR_(0x219)
#define I2C_ITR			            _SFR_(0x21A)
#define I2C_CCRL		            _SFR_(0x21B)
#define I2C_CCRH		            _SFR_(0x21C)
#define I2C_TRISER		            _SFR_(0x21D)
#define I2C_PECR		            _SFR_(0x21E)

/* UART1 */
#define UART1_SR		            _SFR_(0x230)
#define UART1_DR		            _SFR_(0x231)
#define UART1_BRR1		            _SFR_(0x232)
#define UART1_BRR2		            _SFR_(0x233)
#define UART1_CR1		            _SFR_(0x234)
#define UART1_CR2		            _SFR_(0x235)
#define UART1_CR3		            _SFR_(0x236)
#define UART1_CR4		            _SFR_(0x237)
#define UART1_CR5		            _SFR_(0x238)
#define UART1_GTR		            _SFR_(0x239)
#define UART1_PSCR		            _SFR_(0x23A)

/* TIM1 */
#define TIM1_CR1		            _SFR_(0x250)
#define TIM1_CR2		            _SFR_(0x251)
#define TIM1_SMCR		            _SFR_(0x252)
#define TIM1_ETR		            _SFR_(0x253)
#define TIM1_IER		            _SFR_(0x254)
#define TIM1_SR1		            _SFR_(0x255)
#define TIM1_SR2		            _SFR_(0x256)
#define TIM1_EGR		            _SFR_(0x257)
#define TIM1_CCMR1		            _SFR_(0x258)
#define TIM1_CCMR2		            _SFR_(0x259)
#define TIM1_CCMR3		            _SFR_(0x25A)
#define TIM1_CCMR4		            _SFR_(0x25B)
#define TIM1_CCER1		            _SFR_(0x25C)
#define TIM1_CCER2		            _SFR_(0x25D)
#define TIM1_CNTRH		            _SFR_(0x25E)
#define TIM1_CNTRL		            _SFR_(0x25F)
#define TIM1_PSCRH		            _SFR_(0x260)
#define TIM1_PSCRL		            _SFR_(0x261)
#define TIM1_ARRH		            _SFR_(0x262)
#define TIM1_ARRL		            _SFR_(0x263)
#define TIM1_RCR		            _SFR_(0x264)
#define TIM1_CCR1H		            _SFR_(0x265)
#define TIM1_CCR1L		            _SFR_(0x266)
#define TIM1_CCR2H		            _SFR_(0x267)
#define TIM1_CCR2L		            _SFR_(0x268)
#define TIM1_CCR3H		            _SFR_(0x269)
#define TIM1_CCR3L		            _SFR_(0x26A)
#define TIM1_CCR4H		            _SFR_(0x26B)
#define TIM1_CCR4L		            _SFR_(0x26C)
#define TIM1_BKR		            _SFR_(0x26D)
#define TIM1_DTR		            _SFR_(0x26E)
#define TIM1_OISR		            _SFR_(0x26F)

/* TIM2 */
#define TIM2_CR1		            _SFR_(0x300)
#define TIM2_IER		            _SFR_(0x303)
#define TIM2_SR1		            _SFR_(0x304)
#define TIM2_SR2		            _SFR_(0x305)
#define TIM2_EGR		            _SFR_(0x306)
#define TIM2_CCMR1		            _SFR_(0x307)
#define TIM2_CCMR2		            _SFR_(0x308)
#define TIM2_CCMR3		            _SFR_(0x309)
#define TIM2_CCER1		            _SFR_(0x30A)
#define TIM2_CCER2		            _SFR_(0x30B)
#define TIM2_CNTRH		            _SFR_(0x30C)
#define TIM2_CNTRL		            _SFR_(0x30D)
#define TIM2_PSCR		            _SFR_(0x30E)
#define TIM2_ARRH		            _SFR_(0x30F)
#define TIM2_ARRL		            _SFR_(0x310)
#define TIM2_CCR1H		            _SFR_(0x311)
#define TIM2_CCR1L		            _SFR_(0x312)
#define TIM2_CCR2H		            _SFR_(0x313)
#define TIM2_CCR2L		            _SFR_(0x314)
#define TIM2_CCR3H		            _SFR_(0x315)
#define TIM2_CCR3L		            _SFR_(0x316)

/* TIM4 */
#define TIM4_CR1		            _SFR_(0x340)
#define TIM4_IER		            _SFR_(0x343)
#define TIM4_SR			            _SFR_(0x344)
#define TIM4_EGR		            _SFR_(0x345)
#define TIM4_CNTR		            _SFR_(0x346)
#define TIM4_PSCR		            _SFR_(0x347)
#define TIM4_ARR		            _SFR_(0x348)

/* ADC1 */
#define ADC_CSR			            _SFR_(0x400)
#define ADC_CR1			            _SFR_(0x401)
#define ADC_CR2			            _SFR_(0x402)
#define ADC_CR3			            _SFR_(0x403)
#define ADC_DRH			            _SFR_(0x404)
#define ADC_DRL			            _SFR_(0x405)
#define ADC_TDRH		            _SFR_(0x406)
#define ADC_TDRL		            _SFR_(0x407)
#define ADC_HTRH		            _SFR_(0x408)
#define ADC_HTRL		            _SFR_(0x409)
#define ADC_LTRH		            _SFR_(0x40A)
#define ADC_LTRL		            _SFR_(0x40B)
#define ADC_AWSRH		            _SFR_(0x40C)
#define ADC_AWSRL		            _SFR_(0x40D)
#define ADC_AWCRH		            _SFR_(0x40E)
#define ADC_AWCRL		            _SFR_(0x40F)

/* CPU */
#define A				            _CFR_(0x000)
#define PCE				            _CFR_(0x001)
#define PCH				            _CFR_(0x002)
#define PCL				            _CFR_(0x003)
#define XH				            _CFR_(0x004)
#define XL				            _CFR_(0x005)
#define YH				            _CFR_(0x006)
#define YL				            _CFR_(0x007)
#define SPH				            _CFR_(0x008)
#define SPL				            _CFR_(0x009)
#define CCR				            _CFR_(0x00A)
#define CFG_GCR			            _CFR_(0x060)

/* ITC */
#define ITC_SPR1		            _CFR_(0x070)
#define ITC_SPR2		            _CFR_(0x071)
#define ITC_SPR3		            _CFR_(0x072)
#define ITC_SPR4		            _CFR_(0x073)
#define ITC_SPR5		            _CFR_(0x074)
#define ITC_SPR6		            _CFR_(0x075)
#define ITC_SPR7		            _CFR_(0x076)
#define ITC_SPR8		            _CFR_(0x077)

/* SWIM */
#define SWIM_CSR		            _CFR_(0x80)

/* DM */
#define DM_BK1RE		            _CFR_(0x90)
#define DM_BK1RH                    _CFR_(0x91)
#define DM_BK1RL                    _CFR_(0x92)
#define DM_BK2RE                    _CFR_(0x93)
#define DM_BK2RH                    _CFR_(0x94)
#define DM_BK2RL                    _CFR_(0x95)
#define DM_CR1                      _CFR_(0x96)
#define DM_CR2                      _CFR_(0x97)
#define DM_CSR1                     _CFR_(0x98)
#define DM_CSR2                     _CFR_(0x99)
#define DM_ENFCTR                   _CFR_(0x9A)

/* OPTION BYTE */
#define OPT0			            _OPTION_BYTE_(0x00)
#define OPT1			            _OPTION_BYTE_(0x01)
#define NOPT1			            _OPTION_BYTE_(0x02)
#define OPT2			            _OPTION_BYTE_(0x03)
#define NOPT2			            _OPTION_BYTE_(0x04)
#define OPT3			            _OPTION_BYTE_(0x05)
#define NOPT3			            _OPTION_BYTE_(0x06)
#define OPT4			            _OPTION_BYTE_(0x07)
#define NOPT4			            _OPTION_BYTE_(0x08)
#define OPT5			            _OPTION_BYTE_(0x09)
#define NOPT5			            _OPTION_BYTE_(0x0A)

/* Interrupts */
#define TLI_ISR                     0
#define AWU_ISR                     1
#define CLK_ISR                     2
#define EXTI0_ISR                   3
#define EXTI1_ISR                   4
#define EXTI2_ISR                   5
#define EXTI3_ISR                   6
#define EXTI4_ISR                   7
#define SPI_ISR                     10
#define TIM1_OVF_ISR                11
#define TIM1_CC_ISR                 12
#define TIM2_OVF_ISR                13
#define TIM2_CC_ISR                 14
#define UART1_TXC_ISR               17
#define UART1_RXC_ISR               18
#define I2C_ISR                     19
#define ADC1_ISR                    22
#define TIM4_ISR                    23
#define FLASH_ISR                   24

#define FLASH_PAGE_COUNT            128u
#define FLASH_PAGE_SIZE             64u
#define FLASH_PAGE_BLOCK_COUNT      1u
#define FLASH_BLOCK_SIZE            64u
#define EEPROM_PAGE_COUNT           2u
#define EEPROM_PAGE_SIZE            64u
#define EEPROM_PAGE_BLOCK_COUNT     1u
#define EEPROM_BLOCK_SIZE           64u
#define RAM_START                   0x00000000u
#define RAM_END                     0x000003FFu
#define FLASH_START                 0x00008000u
#define FLASH_END                   0x00009FFFu
#define EEPROM_START                0x00004000u
#define EEPROM_END                  0x0000407Fu

#define enable_interrupts()         __asm__("rim");
#define disable_interrupts()        __asm__("sim");
#define rim()                       {__asm__("rim\n");}  /* enable interrupts */
#define nop()                       {__asm__("nop\n");}  /* No Operation */
#define trap()                      {__asm__("trap\n");} /* Trap (soft IT) */
#define sim()                       {__asm__("sim\n");}  /* disable interrupts */
#define wfi()                       {__asm__("wfi\n");}  /* Wait For Interrupt */
#define halt()                      {__asm__("halt\n");} /* Halt */

#endif
