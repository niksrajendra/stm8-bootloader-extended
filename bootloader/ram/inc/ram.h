#ifndef RAM_H
#define RAM_H

#include <stdint.h>

#define FLASH_CR2_OPT           7
#define FLASH_CR2_WPRG          6
#define FLASH_CR2_ERASE         5
#define FLASH_CR2_FPRG          4
#define FLASH_CR2_PRG           0

#define FLASH_NCR2_NOPT         7
#define FLASH_NCR2_NWPRG        6
#define FLASH_NCR2_NERASE       5
#define FLASH_NCR2_NFPRG        4
#define FLASH_NCR2_NPRG         0

#define FLASH_IAPSR_DUL         3
#define FLASH_IAPSR_EOP         2
#define FLASH_IAPSR_PUL         1

#define _MEM_(mem_addr)         (*(volatile uint8_t *)(mem_addr))

void ram_flash_write_block(uint16_t addr, const uint8_t *buf);

#endif /* RAM_H */