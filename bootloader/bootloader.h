#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#ifdef STM8S_003_H
    #define BLOCK_SIZE      64
#else
    #define BLOCK_SIZE      128
#endif

/* application address */
#define BOOT_ADDR           0x8800

/* entry jumper */
#define BOOT_PIN            4
#define BOOT_PIN_IDR        PD_IDR
#define BOOT_PIN_CR1        PD_CR1

#define FLASH_PUKR_KEY1         0x56
#define FLASH_PUKR_KEY2         0xAE

#define STR(x)              #x
#define STRX(x)             STR(x)
#define BOOT()              __asm__("jp " STRX(BOOT_ADDR))

void bootloader_exec(void);
//static void flash_write_block(uint16_t addr, const uint8_t *buf);

#endif  //BOOTLOADER_H