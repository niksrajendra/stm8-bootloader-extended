#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#ifdef STM8S_003_H
    #define BLOCK_SIZE      64
#else
    #define BLOCK_SIZE      128
#endif

#define BOOT_ADDR       0x8400

void bootloader_exec(void);
void flash_write_block(uint16_t addr, const uint8_t *buf);

#endif  //BOOTLOADER_H