#include "dhrystone/dhry.h"
#include "dhrystone/mini_lib.h"
#include "riscv.h"

char *strcpy(char *dst, const char *src) {
    char *d = dst;
    while ((*d++ = *src++) != 0) {
        /* copy */
    }
    return dst;
}

int times(struct tms *buf) {
    uint64_t cycles = r_mcycle64();
    long ticks = (long)cycles;

    if (buf) {
        buf->tms_utime  = ticks;
        buf->tms_stime  = 0;
        buf->tms_cutime = 0;
        buf->tms_cstime = 0;
    }

    return (int)ticks;
}
