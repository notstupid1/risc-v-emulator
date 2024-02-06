
#ifndef OPCODES_H
#define OPCODES_H

// RV32I Base Instruction Set

#define OPC_I_TYPE 0x13
    #define FUNC_3_ADDI 0x00
    #define FUNC_3_SLLI 0x01
    #define FUNC_3_SLTI 0x02
    #define FUNC_3_SLTIU 0x03
    #define FUNC_3_XORI 0x04
    #define FUNC_3_SRI 0x05
        #define IMM_U_7_BITS_SRLI 0x00
        #define IMM_U_7_BITS_SRAI 0x20
    #define FUNC_3_ORI 0x06
    #define FUNC_3_ANDI 0x07

#define OPC_I_TYPE_LOAD 0x03
    #define FUNC_3_LB 0x00
    #define FUNC_3_LH 0x01
    #define FUNC_3_LW 0x02
    #define FUNC_3_LBU 0x04
    #define FUNC_3_LHU 0x05

#define OPC_I_TYPE_JALR 0x67
    #define FUNC_3_JALR 0x00

#define OPC_S_TYPE_STORE 0x23
    #define FUNC_3_SB 0x00
    #define FUNC_3_SH 0x01
    #define FUNC_3_SW 0x02

#define OPC_R_TYPE 0x33
    #define FUNC_3_ADD_SUB 0x00
        #define FUNC_7_ADD 0x00
        #define FUNC_7_SUB 0x20
    #define FUNC_3_SLL 0x01
    #define FUNC_3_SLT 0x02
    #define FUNC_3_SLTU 0x03
    #define FUNC_3_XOR 0x04
    #define FUNC_3_SR 0x05
        #define FUNC_7_SRL 0x00
        #define FUNC_7_SRA 0x20
    #define FUNC_3_OR 0x06
    #define FUNC_3_AND 0x07

#define OPC_B_TYPE 0x53
    #define FUNC_3_BEQ 0x00
    #define FUNC_3_BNE 0x01
    #define FUNC_3_BLT 0x04
    #define FUNC_3_BGE 0x05
    #define FUNC_3_BLTU 0x06
    #define FUNC_3_BGEU 0x07

#define OPC_U_TYPE_LUI 0x37
#define OPC_U_TYPE_AUIPC 0x17

#define OPC_J_TYPE_JAL 0x5f

#define OPC_I_TYPE_SYSTEM 0x73
    #define FUNC_3_CALL_BREAK 0x00
        #define FUNC_12_ECALL 0x000
        #define FUNC_12_EBREAK 0x001

#define OPC_FENCE 0x0f

enum rv32i {LUI, AUIPC, JAL, JALR, BEQ, BNE, BLT, BGE, BLTU, BGEU, LB, LH, LW, LBU, LHU, SB, SH, SW, 
            ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI, ADD, SUB, SLL, SLT, SLTU, XOR, SRL, 
            SRA, OR, AND, FENCE, ECALL, EBREAK};

#endif