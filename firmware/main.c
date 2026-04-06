#include "riscv.h"
#include "uart.h"
#include "printf.h"
#include "dhrystone/dhry.h"
#include "m5ops.h"
#include "sort.h"

#define CLINT_BASE 0x2000000
#define MTIME (CLINT_BASE + 0xbff8)
#define MTIMECMP (CLINT_BASE + 0x4000)

extern uint32_t disable_mtime_int();
extern void enable_mtime_int();
extern void resume_mtime_int(uint32_t mie);
extern void set_mtimecmp(uint64_t time);
extern uint64_t get_mtime();
extern void trap_entry();
extern void timer_init();
extern int main();

void start() {
  uint32_t tmp;

  // set up mtvec
  w_mtvec((uint32_t)trap_entry);

  // init mstatus.MPP to M-mode
  tmp = r_mstatus();
  tmp |= MSTATUS_MPP_M;
  w_mstatus(tmp);

  // init mstatus.MPIE
  tmp = r_mstatus();
  tmp |= MSTATUS_MPIE;
  w_mstatus(tmp); 

  // mepc 
  w_mepc((uint32_t)main); 

  // mret to main
  asm volatile("mret");
}

void pmenu(void) {
  printf("============ menu ============\n");
  printf("a : calculate a big number and insert mtime interrupt\n");
  printf("m : set mtime cmp\n");
  printf("M : test print mcycle\n");
  printf("d : run dhrystone\n");
  printf("T : test print mtime\n");
  printf("e : test m5exit\n");
  printf("E : test status dump\n");
  printf("s : run bubble sort\n");
  printf("S : run merge sort\n");
  printf("==============================\n");
}

int main() {
  timer_init();

  printf("start simple firmware!\n");
  pmenu();

  while (1) {
    char c = uart_getchar();
    if (c == 'a') {
      printf("a : calculate a big number and insert mtime interrupt!\n");
      int a = 0;
      int b = 0;
      set_mtimecmp(get_mtime() + 100000);
      for (int i = 0; i < 20000; i++) {
        a = a + 1;
	b = a + b;
      }
      printf("done!\n");
    } else if (c == 'm') {
      printf("m : set mtime cmp !\n");
      set_mtimecmp(get_mtime() + 100000);
    } else if (c == 'M') {
      printf("M : test print mcycle !\n");
      printf("mcycle : %u\n", r_mcycle());
      printf("mcycle_hi : %u\n", r_mcycleh());
    } else if (c == 'T') {
      printf("T : test print mtime !\n");
      uint64_t mtime = r_mtime64();
      uint32_t mtime_low = mtime;
      uint32_t mtime_hi = mtime >> 32;
      printf("mtime : %u\n", mtime_low);
      printf("mtime_hi : %u\n", mtime_hi);
    } else if (c == 'd') {
      printf("run dhrystone !\n");
      dhrystone_main();
    } else if (c == 'e') {
      printf("test m5 exit !\n");
      m5_exit();
    } else if (c == 'E') {
      printf("test m5 status !\n");
      m5_resetstats();
      m5_dumpstats();
    } else if (c == 's') {
      printf("run bubble sort!\n");
      bubble_sort(); 
    } else if (c == 'S') {
      printf("run merge sort!\n");
      merge_sort();
    }
  }

  return 0;
}

void handle_interrupt(uint32_t code) {
  printf("handle_interrupt !\n");
  if (code == 0x7) {
    printf("timer interrupt !\n");
    set_mtimecmp(0xffffffffffffffff);
  } else {
    printf("unknown interrupt\n");
  }
}

void handle_exception(uint32_t code) {
  printf("handle_exception !\n");
}

void handle_trap() {
  uint32_t mcause, mepc;
  asm volatile("csrr %0, mcause" : "=r"(mcause));
  asm volatile("csrr %0, mepc" : "=r"(mepc));

  if (mcause >> 31) {
    handle_interrupt((mcause << 1) >> 1);
  } else {
    handle_exception(mcause);
    asm volatile("csrr t0, mepc");
    asm volatile("addi t0, t0, 0x4");
    asm volatile("csrw mepc, t0");
  }
}

uint32_t disable_mtime_int() {
    uint32_t original = r_mie();
    uint32_t mie = original & ~(1 << 7);
    w_mie(mie);
    return original;
}

void enable_mtime_int() {
    uint32_t mie = r_mie();
    mie |= (1 << 7);
    w_mie(mie);
}

void resume_mtime_int(uint32_t mie) {
    w_mie(mie);
}

uint64_t get_mtime() {
  volatile uint32_t *mtimel = (volatile uint32_t *)MTIME;
  volatile uint32_t *mtimeh = (volatile uint32_t *)(MTIME + 4);

  uint64_t ret = *mtimeh;
  ret <<= 32;
  ret += *mtimel;

  return ret;
}

void set_mtimecmp(uint64_t time) {
  volatile uint32_t *mtimecmpl = (volatile uint32_t *)MTIMECMP;
  volatile uint32_t *mtimecmph = (volatile uint32_t *)(MTIMECMP + 4);

  uint32_t original = disable_mtime_int();
  *mtimecmpl = (uint32_t)time;
  *mtimecmph = (uint32_t)(time >> 32);
  resume_mtime_int(original);
}

void timer_init() {
  set_mtimecmp(0xffffffffffffffff);
  enable_mtime_int();
}
