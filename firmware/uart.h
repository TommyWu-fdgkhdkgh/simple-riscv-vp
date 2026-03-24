#ifndef __UART_H__
#define __UART_H__
#include <stdbool.h>
#include <stdint.h>

void uart_putchar(char c);

char uart_getchar(void);
#endif
