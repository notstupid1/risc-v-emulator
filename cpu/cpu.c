#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "../mem.h"
#include "reg.h"
#include "cpu.h"
#include "opcodes.h"

void cpu_init(void);
uint32_t fetch_instruction(void);
int decode_instruction(uint32_t);
bool execute_instruction(uint32_t, int);

// All 32 registers of risc-v architecture implemented using array
uint64_t reg[32];

// Program Counter Register
uint64_t PC = MEM_BASE;

// main cpu logic of fetching-decoding-executing instruction
void cpu_run() {
    cpu_init();

    bool halt = false;
    uint32_t  instruction;
    int ins_code;

    while(!halt && (PC < (MEM_BASE + MEM_SIZE))) {
        reg[ZERO] = 0;
        instruction = fetch_instruction();
        //printf("%x ", instruction);
        ins_code = decode_instruction(instruction);
        //printf("%d ", ins_code);
        halt = execute_instruction(instruction, ins_code);
    }
}

void cpu_init() {
    reg[ZERO] = 0;
    reg[SP]   = MEM_BASE + MEM_SIZE;
}

// fetches instruction from memory and increments Program Counter 
uint32_t fetch_instruction() {
    uint32_t ins  =  (uint32_t) mem[PC - MEM_BASE] 
                  |  ((uint32_t) mem[PC - MEM_BASE + 1] << 8)
                  |  ((uint32_t) mem[PC - MEM_BASE + 2] << 16)
                  |  ((uint32_t) mem[PC - MEM_BASE + 3] << 24);
    PC += 4;
    return ins;
}

// decodes instruction via switch case and returns an enum constant
// opcodes and enum costants are defined in opcodes.h file
int decode_instruction(uint32_t ins) {
    uint8_t op_code = (uint8_t) ins & 0x7f;
    uint8_t funct_3 = ( (uint8_t) (ins >> 12) ) & 0x07;
    uint8_t funct_7 = ( (uint8_t) (ins >> 25) ) & 0x7f;
    uint16_t func_12 = ( (uint16_t) (ins >> 20) ) & 0xfff;

    //printf("%x ", op_code);

    // rv32i instructions
    switch (op_code) {
        case OPC_I_TYPE:
            switch (funct_3) {
                case FUNC_3_ADDI:
                    return ADDI;
                case FUNC_3_SLLI:
                    return SLLI;
                case FUNC_3_SLTI:
                    return SLTI;
                case FUNC_3_SLTIU:
                    return SLTIU;
                case FUNC_3_XORI:
                    return XORI;
                case FUNC_3_SRI:
                    switch (funct_7) {
                        case IMM_U_7_BITS_SRLI:
                            return SRLI;
                        case IMM_U_7_BITS_SRAI:
                            return SRAI;
                    }
                case FUNC_3_ORI:
                    return ORI;
                case FUNC_3_ANDI:
                    return ANDI;
            }
        case OPC_I_TYPE_LOAD:
            switch (funct_3) {
                case FUNC_3_LB:
                    return LB;
                case FUNC_3_LH:
                    return LH;
                case FUNC_3_LW:
                    return LW;
                case FUNC_3_LBU:
                    return LBU;
                case FUNC_3_LHU:
                    return LHU;
            }
        case OPC_S_TYPE_STORE:
            switch (funct_3) {
                case FUNC_3_SB:
                    return SB;
                case FUNC_3_SH:
                    return SH;
                case FUNC_3_SW:
                    return SW;
            }
        case OPC_R_TYPE:
            switch (funct_3) {
                case FUNC_3_ADD_SUB:
                    switch (funct_7) {
                        case FUNC_7_ADD:
                            return ADD;
                        case FUNC_7_SUB:
                            return SUB;
                    }
                case FUNC_3_SLL:
                    return SLL;
                case FUNC_3_SLT:
                    return SLT;
                case FUNC_3_SLTU:
                    return SLTU;
                case FUNC_3_XOR:
                    return XOR;
                case FUNC_3_SR:
                    switch (funct_7) {
                        case FUNC_7_SRL:
                            return SRL;
                        case FUNC_7_SRA:
                            return SRA;
                    }
                case FUNC_3_OR:
                    return OR;
                case FUNC_3_AND:
                    return AND;
            }
        case OPC_B_TYPE:
            switch (funct_3) {
                case FUNC_3_BEQ:
                    return BEQ;
                case FUNC_3_BNE:
                    return BNE;
                case FUNC_3_BLT:
                    return BLT;
                case FUNC_3_BGE:
                    return BGE;
                case FUNC_3_BLTU:
                    return BLTU;
                case FUNC_3_BGEU:
                    return BGEU;
            }
        case OPC_U_TYPE_LUI:
            return LUI;
        case OPC_U_TYPE_AUIPC:
            return AUIPC;
        case OPC_J_TYPE_JAL:
            return JAL;
        case OPC_I_TYPE_JALR:
            return JALR;
        case OPC_I_TYPE_SYSTEM:
            switch (funct_3) {
                case FUNC_3_CALL_BREAK:
                    switch (func_12) {
                        case FUNC_12_ECALL:
                            return ECALL;
                        case FUNC_12_EBREAK:
                            return EBREAK;
                    }
            }
        case OPC_FENCE:
            return FENCE;
    }
    return -1;  
}

// executes instruction via switching on enum constant returned by decode_instruction function
bool execute_instruction (uint32_t ins, int ins_code) {
    uint8_t destination_reg = ( (uint8_t) (ins >> 7) ) & 0x1f;
    uint8_t source_reg_1 = ( (uint8_t) (ins >> 15) ) & 0x1f;
    uint8_t source_reg_2 = ( (uint8_t) (ins >> 20) ) & 0x1f;
    int16_t i_immidiate = ( (int16_t) (ins >> 20) ) & 0xfff;
    int8_t shamt = (int8_t)(i_immidiate) & 0x1f;
    int32_t u_immidiate = ( (uint32_t) (ins >> 12) );
    int32_t j_immidiate = (int32_t) 
                            ( ((ins >> 21) & 0x3ff)
                            | (((ins >> 20) & 0x01) << 10)
                            | (((ins >> 12) & 0xff) << 11)
                            | (((ins >> 31) & 0x01) << 19)
                            );
                            
    int16_t b_immidiate = (int16_t)
                            (  ((ins >> 8) & 0x0f)
                            |  (((ins >> 25) & 0x3f) << 4)
                            |  (((ins >> 7) & 0x01) << 10)
                            |  (((ins >> 31) & 0x01) << 11)
                            ) << 1;

    int16_t s_immidiate = (int16_t)
                            (  ((ins >> 7) & 0x1f)
                            |  (((ins >> 25) & 0x7f) << 5)
                            );
    // rv32i instructions
    switch (ins_code) {
        case LUI:
            //printf("0x%x ", u_immidiate);
            reg[destination_reg] = (int64_t)(u_immidiate << 12);
            break;
        case AUIPC:
            reg[destination_reg] = (PC - 4) + (int64_t)(u_immidiate << 12); // possibility of some bugs
            break;
        case JAL:
            reg[destination_reg] = PC;
            PC = (PC - 4) + (int64_t)j_immidiate;
            break;
        case JALR:
            reg[destination_reg] = PC;
            PC = (reg[source_reg_1] + (int64_t)i_immidiate) & 0xfffffffffffffffe;
            break;
        case BEQ:
            PC = ( reg[source_reg_1] == reg[source_reg_2] ) ? (PC - 4) + (int64_t)b_immidiate : PC;
            break;
        case BNE:
            PC = ( reg[source_reg_1] != reg[source_reg_2] ) ? (PC - 4) + (int64_t)b_immidiate : PC;
            break;
        case BLT:
            PC = ( (int64_t)reg[source_reg_1] < (int64_t)reg[source_reg_2] ) ? (PC - 4) + (int64_t)b_immidiate : PC;
            break;
        case BGE:
            PC = ( (int64_t)reg[source_reg_1] >= (int64_t)reg[source_reg_2] ) ? (PC - 4) + (int64_t)b_immidiate : PC;
            break;
        case BLTU:
            PC = ( (uint64_t)reg[source_reg_1] < (uint64_t)reg[source_reg_2] ) ? (PC - 4) + (int64_t)b_immidiate : PC;
            break;
        case BGEU:
            PC = ( (uint64_t)reg[source_reg_1] >= (uint64_t)reg[source_reg_2] ) ? (PC - 4) + (int64_t)b_immidiate : PC;
            break;
        case LB:
            reg[destination_reg] = (int64_t) mem[reg[source_reg_1] + (int64_t)i_immidiate - MEM_BASE];
            break;
        case LH:
            reg[destination_reg] = (int64_t) mem[reg[source_reg_1] + (int64_t)i_immidiate - MEM_BASE]
                                        |    ((int64_t) mem[reg[source_reg_1] + (int64_t)i_immidiate - MEM_BASE + 1] << 8);
            break;
        case LW:
            reg[destination_reg] = (uint64_t) (mem[reg[source_reg_1] + (int64_t)i_immidiate - MEM_BASE + 3] << 24)
                                        |    ((uint64_t) mem[reg[source_reg_1] + (int64_t)i_immidiate - MEM_BASE + 2] << 16)
                                        |    ((uint64_t) mem[reg[source_reg_1] + (int64_t)i_immidiate - MEM_BASE + 1] << 8)
                                        |    ((uint64_t) mem[reg[source_reg_1] + (int64_t)i_immidiate - MEM_BASE + 0] << 0);
            break;
        case LBU:
            reg[destination_reg] = (uint64_t) mem[reg[source_reg_1] + (int64_t)i_immidiate - MEM_BASE];
            break;
        case LHU:
            reg[destination_reg] = (uint64_t) mem[reg[source_reg_1] + (int64_t)i_immidiate - MEM_BASE]
                                        |    ((uint64_t) mem[reg[source_reg_1] + (int64_t)i_immidiate - MEM_BASE + 1] << 8);
            break;
        case SB:
            mem[reg[source_reg_1] + (int64_t)s_immidiate - MEM_BASE] = reg[source_reg_2];
            break;
        case SH:
            mem[reg[source_reg_1] + (int64_t)s_immidiate - MEM_BASE] = (uint8_t)(reg[source_reg_2] & 0xff);
            mem[reg[source_reg_1] + (int64_t)s_immidiate - MEM_BASE + 1] = (uint8_t)((reg[source_reg_2] >> 8) & 0xff);
            break;
        case SW:
            mem[reg[source_reg_1] + (int64_t)s_immidiate - MEM_BASE] = (uint8_t)(reg[source_reg_2] & 0xff);
            mem[reg[source_reg_1] + (int64_t)s_immidiate - MEM_BASE + 1] = (uint8_t)((reg[source_reg_2] >> 8) & 0xff);
            mem[reg[source_reg_1] + (int64_t)s_immidiate - MEM_BASE + 2] = (uint8_t)((reg[source_reg_2] >> 16) & 0xff);
            mem[reg[source_reg_1] + (int64_t)s_immidiate - MEM_BASE + 3] = (uint8_t)((reg[source_reg_2] >> 24) & 0xff);
            break;
        case ADDI:
            reg[destination_reg] = reg[source_reg_1] + (int64_t)i_immidiate;
            break;
        case SLTI:
            reg[destination_reg] = reg[source_reg_1] < (int64_t)i_immidiate ? 1 : 0;
            break;
        case SLTIU:
            reg[destination_reg] = (uint64_t)reg[source_reg_1] < (int64_t)i_immidiate ? 1 : 0;
            break;
        case XORI:
            reg[destination_reg] = reg[source_reg_1] ^ (int64_t)i_immidiate;
            break;
        case ORI:
            reg[destination_reg] = reg[source_reg_1] | (int64_t)i_immidiate;
            break;
        case ANDI:
            reg[destination_reg] = reg[source_reg_1] & (int64_t)i_immidiate;
            break;
        case SLLI:
            reg[destination_reg] = reg[source_reg_1] << shamt;
            break;
        case SRLI:
            reg[destination_reg] = reg[source_reg_1] >> shamt;
            break;
        case SRAI:
            reg[destination_reg] = (int64_t)reg[source_reg_1] >> shamt;
            break;
        case ADD:
            reg[destination_reg] = reg[source_reg_1] + reg[source_reg_2];
            break;
        case SUB:
            reg[destination_reg] = reg[source_reg_2] - reg[source_reg_1];
            break;
        case SLT:
            reg[destination_reg] = (int64_t)reg[source_reg_1] < (int64_t)reg[source_reg_2] ? 1 : 0;
            break;
        case SLTU:
            reg[destination_reg] = (uint64_t)reg[source_reg_1] < (uint64_t)reg[source_reg_2] ? 1 : 0;
            break;
        case XOR:
            reg[destination_reg] = reg[source_reg_1] ^ reg[source_reg_2];
            break;
        case OR:
            reg[destination_reg] = reg[source_reg_1] | reg[source_reg_2];
            break;
        case AND:
            reg[destination_reg] = reg[source_reg_1] & reg[source_reg_2];
            break;    
        case SLL:
            reg[destination_reg] = reg[source_reg_1] << (uint8_t)(reg[source_reg_2] & 0x1f);
            break;
        case SRL:
            reg[destination_reg] = reg[source_reg_1] >> (uint8_t)(reg[source_reg_2] & 0x1f);
            break;
        case SRA:
            reg[destination_reg] = (int64_t)reg[source_reg_1] >> (uint8_t)(reg[source_reg_2] & 0x1f);
            break;
    }
    return false;
}

void dump_mem(void) {
    for (size_t i = 0; i < MEM_SIZE; i += 16)
    {
        printf("0x%16.16x    ", i);
        for (size_t j = 0; j < 16; j++)
        {
            printf("0x%2.2x  ", mem[i + j]);
        }
        printf("\n");
    }
    
}

void dump_reg(void) {
    for (size_t i = 0; i < 32; i += 4)
    {
        for (size_t j = 0; j <  4; j++)
        {
            printf("%s :\t0x%16.16llx\t", reg_name[i + j], reg[i + j]);
        }
        printf("\n");
    }
    
}