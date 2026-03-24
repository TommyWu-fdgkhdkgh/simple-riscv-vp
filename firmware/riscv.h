#ifndef __RISCV_H__
#define __RISCV_H__

#include <stdint.h>

#define MSTATUS_MPP_MASK (3L << 11)  // previous mode.
#define MSTATUS_MPP_M (3L << 11)
#define MSTATUS_MPP_S (1L << 11)
#define MSTATUS_MPP_U (0L << 11)
#define MSTATUS_MPIE (1L << 7)
#define MSTATUS_MIE (1L << 3)  // machine-mode interrupt enable.

// which hart (core) is this?
static inline uint32_t r_mhartid() {
    uint32_t x;
    asm volatile("csrr %0, mhartid" : "=r"(x));
    return x;
}

static inline uint32_t r_mstatus() {
    uint32_t x;
    asm volatile("csrr %0, mstatus" : "=r"(x));
    return x;
}

static inline void w_mstatus(uint32_t x) {
    asm volatile("csrw mstatus, %0" : : "r"(x));
}

// machine exception program counter, holds the
// instruction address to which a return from
// exception will go.
static inline void w_mepc(uint32_t x) {
    asm volatile("csrw mepc, %0" : : "r"(x));
}

// Machine-mode interrupt vector
static inline void w_mtvec(uint32_t x) {
    asm volatile("csrw mtvec, %0" : : "r"(x));
}

// Machine-mode Interrupt Enable
#define MIE_MEIE (1L << 11)  // external
#define MIE_MTIE (1L << 7)   // timer
#define MIE_MSIE (1L << 3)   // software
static inline uint32_t r_mie() {
    uint32_t x;
    asm volatile("csrr %0, mie" : "=r"(x));
    return x;
}

static inline void w_mie(uint32_t x) {
    asm volatile("csrw mie, %0" : : "r"(x));
}

#endif
