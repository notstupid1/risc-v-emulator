#include <stdio.h>
#include <stdint.h>
#include "./cpu/cpu.h"
#include "mem.h"
#include "asm_lib_func.h"

int main (int argc, char *argv[]) {
    lay_in_ram( lui(a0, 0x80000) );

    lay_in_ram( lr(t0, 30) );
    lay_in_ram( addi(a2, t0, 30) );
    lay_in_ram( slti(a3, a2, 70) );
    lay_in_ram( lr(t1, 30) );
    lay_in_ram( add(t2, t0, t1) );
    lay_in_ram( slli(a4, a3, 5) );
    lay_in_ram( lui(t3, -0x1443) );
    lay_in_ram( sw(0x60, a0, t3) );
    lay_in_ram( lw(t4, a0, 0x60) );

    cpu_run();
    //dump_mem();
    dump_reg();
    printf("Running");

    return 0;
}