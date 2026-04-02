void m5_exit() {
  __asm__("addi a0, x0, 0");
  __asm__("addi a1, x0, 0");
  __asm__("addi a2, x0, 0");
  __asm__("addi a3, x0, 0");
  __asm__(".word 0x4200007B");
}

void m5_resetstats() {
  __asm__("addi a0, x0, 0");
  __asm__("addi a1, x0, 0");
  __asm__("addi a2, x0, 0");
  __asm__("addi a3, x0, 0");
  __asm__(".word 0x8000007B");
}

void m5_dumpstats() {
  __asm__("addi a0, x0, 0");
  __asm__("addi a1, x0, 0");
  __asm__("addi a2, x0, 0");
  __asm__("addi a3, x0, 0");
  __asm__(".word 0x8200007B");
}

void m5_dumpresetstats() {
  __asm__("addi a0, x0, 0");
  __asm__("addi a1, x0, 0");
  __asm__("addi a2, x0, 0");
  __asm__("addi a3, x0, 0");
  __asm__(".word 0x8400007B");
}
