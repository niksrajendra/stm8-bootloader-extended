#include <stm8s003.h>
#include <gpio.h>
#include <spi.h>
#include <ram.h>
#include <uart.h>
#include <timer.h>
#include "bootloader.h"
#include "stm8_interrupt_vector.h"




static uint8_t CRC;
static uint8_t ivt[128];
static uint8_t f_ram[128];
static uint8_t rx_buffer[BLOCK_SIZE];
static volatile uint8_t RAM_SEG_LEN;

static void serial_send_ack(void);
static void serial_read_block(uint8_t *dest);
inline void serial_send_nack();

void hse_enable(void);

static void (*flash_write_block)(uint16_t addr, const uint8_t *buf) =
        (void (*)(uint16_t, const uint8_t *)) f_ram;

/**
 * Write RAM_SEG section length into RAM_SEG_LEN
 */
inline void get_ram_section_length() {
    __asm__("mov _RAM_SEG_LEN, #l_RAM_SEG");
}

/**
 * Send ACK response
 */
static void serial_send_ack() {
    uart_write(0xAA,1);
    uart_write(0xBB,1);
}

/**
 * Send NACK response (CRC mismatch)
 */
inline void serial_send_nack() {
    uart_write(0xDE,1);
    uart_write(0xAD,1);
}

/**
 * Calculate CRC-8-CCIT.
 * Polynomial: x^8 + x^2 + x + 1 (0x07)
 *
 * @param data input byte
 * @param crc initial CRC
 * @return CRC value
 */
inline uint8_t crc8_update(uint8_t data, uint8_t crc) {
    crc ^= data;
    for (uint8_t i = 0; i < 8; i++)
        crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : crc << 1;
    return crc;
}

/**
 * Read BLOCK_SIZE bytes from UART
 *
 * @param dest destination buffer
 */
static void serial_read_block(uint8_t *dest) {
    serial_send_ack();
    for (uint8_t i = 0; i < BLOCK_SIZE; i++) {
        uint8_t rx = uart_read(1);
        dest[i] = rx;
        CRC = crc8_update(rx, CRC);
    }
}

void hse_enable(void) {
    /* Enable HSE crystal oscillator */
    //CLK_ECKRbits.HSEEN = 1u;
    CLK_ECKR |= 0x01;
    while (!(CLK_ECKR & 0x02));
    /* Switch master clock to HSE */
    CLK_SWR = 0xB4;
    while (!(CLK_SWCR & (1 << 3)));
    CLK_SWCR |= (1 << 1);
}

/**
 * Enter bootloader and perform firmware update
 */
inline void bootloader_exec(void) {
    uint8_t chunks, crc_rx;
    uint16_t addr = BOOT_ADDR;

    /* enter bootloader */
    for (;;) {
        uint8_t rx = uart_read(1);
        if (rx != 0xDE) continue;
        rx = uart_read(1);
        if (rx != 0xAD) continue;
        rx = uart_read(1);
        if (rx != 0xBE) continue;
        rx = uart_read(1);
        if (rx != 0xEF) continue;
        chunks = uart_read(1);
        crc_rx = uart_read(1);
        rx = uart_read(1);
        if (crc_rx != rx)
            continue;
		uart_write('b',1);
		uart_write('o',1);
		uart_write('o',1);
		uart_write('t',1);
        break;
    }

#if !RELOCATE_IVT
    /* get application interrupt table */
    serial_read_block(ivt);
    chunks--;
    #if BLOCK_SIZE == 64
    chunks--;
    serial_read_block(ivt + BLOCK_SIZE);
    #endif
#endif

    /* unlock flash */
    FLASH_PUKR = FLASH_PUKR_KEY1;
    FLASH_PUKR = FLASH_PUKR_KEY2;
    while (!(FLASH_IAPSR & (1 << FLASH_IAPSR_PUL)));

    /* get main firmware */
    for (uint8_t i = 0; i < chunks; i++) {
        serial_read_block(rx_buffer);
		//serial_verify_block(rx_buffer);
        flash_write_block(addr, rx_buffer);
        addr += BLOCK_SIZE;
    }

    /* verify CRC */
    if (CRC != crc_rx) {
        serial_send_nack();
        for (;;) ;
    }

#if !RELOCATE_IVT
    /* overwrite vector table preserving the reset interrupt */
    *(uint32_t *) ivt = *(uint32_t *) (0x8000);
    flash_write_block(0x8000, ivt);
    #if BLOCK_SIZE == 64
    flash_write_block(0x8000 + BLOCK_SIZE, ivt + BLOCK_SIZE);
    #endif
#endif

    /* lock flash */
    FLASH_IAPSR &= ~(1 << FLASH_IAPSR_PUL);

    serial_send_ack();

    /* reboot */
    for (;;) ;
}

/**
 * Copy ram_flash_write_block routine into RAM
 */
inline void ram_cpy() {
    get_ram_section_length();
    for (uint8_t i = 0; i < RAM_SEG_LEN; i++)
        f_ram[i] = ((uint8_t *) ram_flash_write_block)[i];
}

void main() {
	hse_enable();
    BOOT_PIN_CR1 = 1 << BOOT_PIN;
    if (!(BOOT_PIN_IDR & (1 << BOOT_PIN))) {
        /* execute bootloader */
        //CLK_CKDIVR = 0;
        ram_cpy();
        iwdg_init();
        uart_init(115200, DATA_8_BIT_STOP_1_BIT);
        bootloader_exec();
    } else {
        /* jump to application */
        BOOT_PIN_CR1 = 0x00;
        BOOT();
    }
}