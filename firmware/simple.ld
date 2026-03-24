OUTPUT_ARCH( "riscv" )
ENTRY(_start)

RAM_ADDR   = 0x80000000;
RAM_LENGTH = 0x10000000;
ROM_ADDR   = 0x90000000;
ROM_LENGTH = 0x10000000;

MEMORY
{
  RAM (rw) : ORIGIN = RAM_ADDR, LENGTH = RAM_LENGTH
  ROM (rx) : ORIGIN = ROM_ADDR, LENGTH = ROM_LENGTH
}

SECTIONS
{
  .text : {
    *(.text.start);
    PROVIDE(_text_start = .);
    *(.text.init) *(.text .text.*)
    PROVIDE(_text_end = .);
  } > ROM

  . = ALIGN(0x1000);
  etext = .;

  .sdata : {
    _gp = . + 0x800;
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2)
    *(.srodata*)
    *(.sdata .sdata.* .gnu.linkonce.s.*)
  }

  .rodata : {
    PROVIDE(_rodata_start = .);
    *(.rodata .rodata.*)
    PROVIDE(_rodata_end = .);
  } > ROM
  

  .data : {
    . = ALIGN(4096);
    PROVIDE(_data_start = .);
    *(.sdata .sdata.*) *(.data .data.*)
    PROVIDE(_data_end = .);
  } > RAM AT > ROM

  .bss :{
    PROVIDE(_bss_start = .);
    *(.sbss .sbss.*) *(.bss .bss.*)
    PROVIDE(_bss_end = .);
  } > RAM

  PROVIDE(_memory_start = ORIGIN(RAM));
  PROVIDE(_memory_end = ORIGIN(RAM) + LENGTH(RAM));
  stack_top = ORIGIN(RAM) + LENGTH(RAM);

  . += 0x8000;
  end = .;
}
