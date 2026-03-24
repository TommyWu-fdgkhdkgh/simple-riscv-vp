#ifndef __PRINTF_H__
#define __PRINTF_H__

#include <stdarg.h>

int printf(const char *format, ...);

int sprintf(char *out, const char *format, ...);

int snprintf(char *buf, unsigned int count, const char *format, ...);

#endif
