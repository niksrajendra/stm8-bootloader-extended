#include <stm8s003.h>
#include <gpio.h>
#include <spi.h>
#include <ram.h>
#include <uart.h>
#include "bootloader.h"

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