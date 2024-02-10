#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstdio>

#define pat(val, a, b) (((1 << (a - b + 1)) - 1) & (val >> b))
#define imm_z       pat(inst,19,15)
#define opcode      pat(inst,6,0)
#define funct3      pat(inst,14,12)
#define funct7      pat(inst,31,25)
#define funct5      pat(inst,31,27)
#define rd          pat(inst,11,7)
#define rs1         pat(inst,19,15)
#define rs2         pat(inst,24,20)
#define imm_i       pat(inst,31,20)
#define imm_s       (pat(inst,31,25) << 5) | pat(inst,11,7)
#define imm_b       (pat(inst,31,31) << 12) | (pat(inst,7,7) << 11) | (pat(inst,30,25) << 5) | (pat(inst,11,8) << 1)
#define imm_j       (pat(inst,31,31) << 20) | (pat(inst,19,12) << 12) | (pat(inst,20,20) << 11) | (pat(inst,30,21) << 1)
#define imm_u       pat(inst,31,12)
#define csr_addr    imm_i

namespace rv32ima{
    void disassemble(uint32_t inst);
}