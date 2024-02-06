#include <stdint.h>
#include "mem.h"
#include "asm_lib_func.h"

uint64_t curr_add = 0;

void lay_in_ram(uint32_t ins) {
    mem[curr_add + 0] = (uint8_t) ins & 0xff;
    mem[curr_add + 1] = (uint8_t) (ins >> 8) & 0xff;
    mem[curr_add + 2] = (uint8_t) (ins >> 16) & 0xff;
    mem[curr_add + 3] = (uint8_t) (ins >> 24) & 0xff;

    curr_add += 4;
}