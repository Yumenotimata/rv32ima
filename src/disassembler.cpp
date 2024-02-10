#include "../inc/disassembler.hpp"

namespace rv32ima{
    void disassemble(uint32_t inst){
        switch(opcode)
        {
            case 0x33: // R-type
                switch(funct3)
                {
                    case 0x0: // add / sub
                        if(funct7 == 0x00) printf(    "add x%d, x%d, x%d\n", rd, rs1, rs2); else printf(    "sub x%d, x%d, x%d\n", rd, rs1, rs2); break;
                    case 0x1: // sll
                        printf(    "sll x%d, x%d, x%d\n", rd, rs1, rs2); break;
                    case 0x2: // slt
                        printf(    "slt x%d, x%d, x%d\n", rd, rs1, rs2); break;
                    case 0x3: // sltu
                        printf(    "sltu x%d, x%d, x%d\n", rd, rs1, rs2); break;
                    case 0x4: // xor
                        printf(    "xor x%d, x%d, x%d\n", rd, rs1, rs2); break;
                    case 0x5: // srl
                        printf(    "srl x%d, x%d, x%d\n", rd, rs1, rs2); break;
                    case 0x6: // or
                        printf(    "or x%d, x%d, x%d\n", rd, rs1, rs2); break;
                    case 0x7: // and
                        printf(    "and x%d, x%d, x%d\n", rd, rs1, rs2); break;
                }
                break;
            case 0x13: // I-type
                switch(funct3)
                {
                    case 0x0: // addi
                        printf(    "addi x%d, x%d, %x\n", rd, rs1, imm_i); break;
                    case 0x1: // slli
                        printf(    "slli x%d, x%d, %x\n", rd, rs1, imm_z); break;
                    case 0x2: // slti
                        printf(    "slti x%d, x%d, %x\n", rd, rs1, imm_i); break;
                    case 0x3: // sltiu
                        printf(    "sltiu x%d, x%d, %x\n", rd, rs1, imm_i); break;
                    case 0x4: // xori
                        printf(    "xori x%d, x%d, %x\n", rd, rs1, imm_i); break;
                    case 0x5: // srli / srai
                        if(funct7 == 0x00) printf(    "srli x%d, x%d, %x\n", rd, rs1, imm_z); else printf(    "srai x%d, x%d, %x\n", rd, rs1, imm_z); break;
                    case 0x6: // ori
                        printf(    "ori x%d, x%d, %x\n", rd, rs1, imm_i); break;
                    case 0x7: // andi
                        printf(    "andi x%d, x%d, %x\n", rd, rs1, imm_i); break;
                }
                break;
            case 0x37: // lui
                printf(    "lui x%d, %x\n", rd, imm_u); break;
            case 0x17: // auipc
                printf(    "auipc x%d, %x\n", rd, imm_u); break;
            case 0x6f: // jal
                printf(    "jal x%d, %x\n", rd, imm_j); break;
            case 0x67: // jalr
                printf(    "jalr x%d, x%d, %x\n", rd, rs1, imm_i); break;
            case 0x63: // B-type
                switch(funct3)
                {
                    case 0x0: // beq
                        printf(    "beq x%d, x%d, %x\n", rs1, rs2, imm_b); break;
                    case 0x1: // bne
                        printf(    "bne x%d, x%d, %x\n", rs1, rs2, imm_b); break;
                    case 0x4: // blt
                        printf(    "blt x%d, x%d, %x\n", rs1, rs2, imm_b); break;
                    case 0x5: // bge
                        printf(    "bge x%d, x%d, %x\n", rs1, rs2, imm_b); break;
                    case 0x6: // bltu
                        printf(    "bltu x%d, x%d, %x\n", rs1, rs2, imm_b); break;
                    case 0x7: // bgeu
                        printf(    "bgeu x%d, x%d, %x\n", rs1, rs2, imm_b); break;
                }
                break;
            case 0x03: // load
                switch(funct3)
                {
                    case 0x0: // lb
                        printf(    "lb x%d, %x(x%d)\n", rd, imm_i, rs1); break;
                    case 0x1: // lh
                        printf(    "lh x%d, %x(x%d)\n", rd, imm_i, rs1); break;
                    case 0x2: // lw
                        printf(    "lw x%d, %x(x%d)\n", rd, imm_i, rs1); break;
                    case 0x4: // lbu
                        printf(    "lbu x%d, %x(x%d)\n", rd, imm_i, rs1); break;
                    case 0x5: // lhu
                        printf(    "lhu x%d, %x(x%d)\n", rd, imm_i, rs1); break;
                }
                break;
            case 0x23: // S-type
                switch(funct3)
                {
                    case 0x0: // sb
                        printf(    "sb x%d, %x(x%d)\n", rs2, imm_s, rs1); break;
                    case 0x1: // sh
                        printf(    "sh x%d, %x(x%d)\n", rs2, imm_s, rs1); break;
                    case 0x2: // sw
                        printf(    "sw x%d, %x(x%d)\n", rs2, imm_s, rs1); break;
                }
                break;
            case 0x73: // csr
                switch(funct3)
                {
                    case 0x0: // ecall
                        switch(funct7)
                        {
                            case 0x0: // ecall
                                printf(    "ecall\n"); break;
                            case 0x18: // ebreak
                                printf(    "mret\n"); break;
                        }
                    case 0x1: // csrrw
                        printf(    "csrrw x%d, %x, x%d\n", rd, csr_addr, rs1); break;
                    case 0x2: // csrrs
                        printf(    "csrrs x%d, %x, x%d\n", rd, csr_addr, rs1); break;
                    case 0x3: // csrrc
                        printf(    "csrrc x%d, %x, x%d\n", rd, csr_addr, rs1); break;
                    case 0x5: // csrrwi
                        printf(    "csrrwi x%d, %x, %x\n", rd, csr_addr, imm_i); break;
                    case 0x6: // csrrsi
                        printf(    "csrrsi x%d, %x, %x\n", rd, csr_addr, imm_i); break;
                    case 0x7: // csrrci
                        printf(    "csrrci x%d, %x, %x\n", rd, csr_addr, imm_i); break;
                }
                break;
            case 0x0f: // fence
                printf(    "fence\n"); break;
            default:
                printf(    "unknown instruction 0x%08x\n", inst);
                exit(1);        
        }
    }
}