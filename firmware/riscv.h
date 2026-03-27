#ifndef __RISCV_H__
#define __RISCV_H__

#include <stdint.h>
#include "printf.h"

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

static inline uint32_t r_mcycle(void) {
    uint32_t x;
    __asm__ volatile ("csrr %0, mcycle" : "=r"(x));
    return x;
}

static inline uint32_t r_mcycleh(void) {
    uint32_t x;
    __asm__ volatile ("csrr %0, mcycleh" : "=r"(x));
    return x;
}

static inline uint64_t r_mcycle64(void) {
    uint32_t lo, hi, hi2;
    do {
        hi  = r_mcycleh();
        lo  = r_mcycle();
        hi2 = r_mcycleh();
    } while (hi != hi2);
    // XXX : Does this affect the experiment result ?
    return ((uint64_t)hi << 32) | (uint64_t)lo;
}

static inline uint32_t r_mtime(void) {
    uint32_t x;
    __asm__ volatile ("csrr %0, time" : "=r"(x));
    return x;
}

static inline uint32_t r_mtimeh(void) {
    uint32_t x;
    __asm__ volatile ("csrr %0, timeh" : "=r"(x));
    return x;
}

static inline uint64_t r_mtime64(void) {
  uint32_t lo, hi, hi2;
  do {
      hi  = r_mtimeh();
      lo  = r_mtime();
      hi2 = r_mtimeh();
  } while (hi != hi2);
  // XXX : Does this affect the experiment result ?
  return ((uint64_t)hi << 32) | (uint64_t)lo;
}

#endif
