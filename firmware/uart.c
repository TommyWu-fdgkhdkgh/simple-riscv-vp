// TODO : driver for simple uart

#include <stdbool.h>
#include <stdint.h>
#include "uart.h"

#define SIMPLE_UART_ADDR (0x10000000)

void uart_putchar(char c) {
  *(volatile uint32_t *)SIMPLE_UART_ADDR = (uint32_t) c;
}

char uart_getchar(void) {
  return *(volatile uint32_t *)SIMPLE_UART_ADDR;
}
