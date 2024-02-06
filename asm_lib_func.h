#define zero 0
#define ra 1
#define sp 2
#define gp 3
#define tp 4
#define t0 5
#define t1 6
#define t2 7
#define s0 8
#define s1 9
#define a0 10
#define a1 11
#define a2 12
#define a3 13
#define a4 14
#define a5 15
#define a6 16
#define a7 17
#define s2 18
#define s3 19
#define s4 20
#define s5 21
#define s6 22
#define s7 23
#define s8 24
#define s9 25
#define s10 26
#define s11 27
#define t3 28
#define t4 29
#define t5 30
#define t6 31

#define lr(rd, imm) ((uint32_t) (imm << 20) | (0x0 << 15) | (0x0 << 12) | (rd << 7) | (0x13))
#define addi(rd, rs1, imm) ((uint32_t) (imm << 20) | (rs1 << 15) | (0x0 << 12) | (rd << 7) | (0x13))
#define slti(rd, rs1, imm) ((uint32_t) (imm << 20) | (rs1 << 15) | (0x2 << 12) | (rd << 7) | (0x13))
#define sltiu(rd, rs1, imm) ((uint32_t) (imm << 20) | (rs1 << 15) | (0x3 << 12) | (rd << 7) | (0x13))
#define xori(rd, rs1, imm) ((uint32_t) (imm << 20) | (rs1 << 15) | (0x4 << 12) | (rd << 7) | (0x13))
#define ori(rd, rs1, imm) ((uint32_t) (imm << 20) | (rs1 << 15) | (0x6 << 12) | (rd << 7) | (0x13))
#define andi(rd, rs1, imm) ((uint32_t) (imm << 20) | (rs1 << 15) | (0x7 << 12) | (rd << 7) | (0x13))
#define slli(rd, rs1, imm) ((uint32_t) ((imm & 0x1f) << 20) | (rs1 << 15) | (0x1 << 12) | (rd << 7) | (0x13))
#define srli(rd, rs1, imm) ((uint32_t) ((imm & 0x1f) << 20) | (rs1 << 15) | (0x5 << 12) | (rd << 7) | (0x13))
#define srai(rd, rs1, imm) ((uint32_t) (((imm >> 5) & 0x1f) << 25) | ((imm & 0x1f) << 20) | (rs1 << 15) | (0x5 << 12) | (rd << 7) | (0x13))

#define add(rd, rs1, rs2) ((uint32_t) (0x00 << 25) | (rs2 << 20) | (rs1 << 15) | (0x0 << 12) | (rd << 7) | (0x33))

#define lw(rd, rs1, imm) ((uint32_t) (imm << 20) | (rs1 << 15) | (0x2 << 12) | (rd << 7) | (0x03))

#define sb(imm, rs1, rs2) ((uint32_t) (((imm >> 5) & 0x7f) << 25) | (rs2 << 20) | (rs1 << 15) | (0x0 << 12) | ((imm & 0x1f) << 7) | (0x23))
#define sw(imm, rs1, rs2) ((uint32_t) (((imm >> 5) & 0x7f) << 25) | (rs2 << 20) | (rs1 << 15) | (0x2 << 12) | ((imm & 0x1f) << 7) | (0x23))

#define lui(rd, imm) ((uint32_t) (imm << 12) | (rd << 7) | (0x37))

void lay_in_ram(uint32_t);