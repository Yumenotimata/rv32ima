#include "../inc/proccessor.hpp"
#include "../inc/bus.hpp"
#include "../inc/csr.hpp"

namespace rv32ima{
    processor_t::processor_t(std::unique_ptr<bus_t> &&bus) 
        : bus(std::move(bus)), pc(0x80000000)
    {

    }

    processor_t::~processor_t(){

    }

    void processor_t::step(){
        fetch();

        if(bus->trap.is_valid()){
            printf("trap \n");
            bus->trap.disable();
        }

        execute();
        pc += 4;
    }

    void processor_t::fetch(){
        inst = bus->read32(true, pc);
    }

    void processor_t::execute(){
        
        // printf("[0x%08x] ", pc);
        // disassemble(inst);

        switch(opcode)
        {
            case 0x03:
                switch(funct3)
                {
                    case 0x0: // lb
                        reg[rd] = sext(bus->read8(reg[rs1] + sext(imm_i, 11)), 7); break;
                    case 0x1: // lh
                        reg[rd] = sext(bus->read16(true, reg[rs1] + sext(imm_i, 11)), 15); break;
                    case 0x2: // lw
                        reg[rd] = bus->read32(true, reg[rs1] + sext(imm_i, 11)); break;
                    case 0x4: // lbu
                        reg[rd] = bus->read8(reg[rs1] + sext(imm_i, 11)); break;
                    case 0x5: // lhu
                        reg[rd] = bus->read16(true, reg[rs1] + sext(imm_i, 11)); break;
                }
                break;
            case 0x23:
                switch(funct3)
                {
                    case 0x0: // sb
                        bus->write8(reg[rs1] + sext(imm_s, 11), reg[rs2]); break;
                    case 0x1: // sh
                        bus->write16(true, reg[rs1] + sext(imm_s, 11), reg[rs2]); break;
                    case 0x2: // sw
                        bus->write32(true, reg[rs1] + sext(imm_s, 11), reg[rs2]); break;
                }
                break;
            case 0x33:
                switch(funct3)
                {
                    case 0x0: // add / sub
                        if(funct7 == 0x0) reg[rd] = reg[rs1] + reg[rs2]; else reg[rd] = reg[rs1] - reg[rs2]; break;
                    case 0x1: // sll
                        reg[rd] = reg[rs1] << (reg[rs2] & 0x1f); break;
                    case 0x2: // slt
                        reg[rd] = (int32_t)reg[rs1] < (int32_t)reg[rs2]; break;
                    case 0x3: // sltu
                        reg[rd] = (uint32_t)reg[rs1] < (uint32_t)reg[rs2]; break;
                    case 0x4: // xor
                        reg[rd] = reg[rs1] ^ reg[rs2]; break;
                    case 0x5: // srl / sra
                        if(funct7 == 0x0) reg[rd] = reg[rs1] >> (reg[rs2] & 0x1f); else reg[rd] = (int32_t)reg[rs1] >> (reg[rs2] & 0x1f); break;
                    case 0x6: // or
                        reg[rd] = reg[rs1] | reg[rs2]; break;
                    case 0x7: // and
                        reg[rd] = reg[rs1] & reg[rs2]; break;
                }
                break; 
            case 0x13:
                switch(funct3)
                {
                    case 0x0: // addi
                        reg[rd] = reg[rs1] + sext(imm_i, 11); break;
                    case 0x1: // slli / srli
                        if(funct7 == 0x0) reg[rd] = reg[rs1] << (imm_i & 0x1f); else reg[rd] = reg[rs1] >> (imm_i & 0x1f); break;
                    case 0x2: // slti
                        reg[rd] = (int32_t)reg[rs1] < (int32_t)sext(imm_i, 11); break;
                    case 0x3: // sltiu
                        reg[rd] = (uint32_t)reg[rs1] < (uint32_t)sext(imm_i, 11); break;
                    case 0x4: // xori
                        reg[rd] = reg[rs1] ^ sext(imm_i, 11); break;
                    case 0x5: // srli / srai
                        if(funct7 == 0x0) reg[rd] = reg[rs1] >> (imm_i & 0x1f); else reg[rd] = (int32_t)reg[rs1] >> (imm_i & 0x1f); break;
                    case 0x6: // ori
                        reg[rd] = reg[rs1] | sext(imm_i, 11); break;
                    case 0x7: // andi
                        reg[rd] = reg[rs1] & sext(imm_i, 11); break;
                }
                break;
            case 0x37: // lui
                reg[rd] = imm_u << 12; break;
            case 0x17: // auipc
                reg[rd] = pc + (imm_u << 12); break;
            case 0x6f: // jal
                reg[rd] = pc + 4; pc += sext(imm_j, 20) - 4; break;
            case 0x67: // jalr
                { uint32_t _pc = pc + 4; pc = ((reg[rs1] + sext(imm_i, 11)) & (~1)) - 4; reg[rd] = _pc; break; }
            case 0x63: // B-type
                switch(funct3)
                {
                    case 0x0: // beq
                        if(reg[rs1] == reg[rs2]) pc += sext(imm_b, 12) - 4; break;
                    case 0x1: // bne
                        if(reg[rs1] != reg[rs2]) pc += sext(imm_b, 12) - 4; break;
                    case 0x4: // blt
                        if((int32_t)reg[rs1] < (int32_t)reg[rs2]) pc += sext(imm_b, 12) - 4; break;
                    case 0x5: // bge
                        if((int32_t)reg[rs1] >= (int32_t)reg[rs2]) pc += sext(imm_b, 12) - 4; break;
                    case 0x6: // bltu
                        if((uint32_t)reg[rs1] < (uint32_t)reg[rs2]) pc += sext(imm_b, 12) - 4; break;
                    case 0x7: // bgeu
                        if((uint32_t)reg[rs1] >= (uint32_t)reg[rs2]) pc += sext(imm_b, 12) - 4; break;
                }
                break;
            case 0x73: // csr
                switch(funct3)
                {
                    case 0x0: // ecall / mret
                        switch(funct7)
                        {
                            case 0x0: // ecall
                                csr[mcause] = (uint32_t)trap_code_t::ECALL_FROM_MMODE; pc = csr[mtvec] - 4; break;
                            case 0x18: // mret
                                pc = csr[mepc] - 4; break;
                            default:
                                printf("Unknown instruction 0x%08x\n", inst);
                                exit(1);
                        }
                        break;
                    case 0x1: // csrrw
                        reg[rd] = csr[csr_addr]; csr[csr_addr] = reg[rs1]; break;
                    case 0x2: // csrrs
                        reg[rd] = csr[csr_addr]; csr[csr_addr] |= reg[rs1]; break;
                    case 0x3: // csrrc
                        reg[rd] = csr[csr_addr]; csr[csr_addr] &= ~reg[rs1]; break;
                    case 0x5: // csrrwi
                        reg[rd] = csr[csr_addr]; csr[csr_addr] = imm_i; break;
                    case 0x6: // csrrsi
                        reg[rd] = csr[csr_addr]; csr[csr_addr] |= imm_i; break;
                    case 0x7: // csrrci
                        reg[rd] = csr[csr_addr]; csr[csr_addr] &= ~imm_i; break;
                }
                break;
            case 0x0f: //fence
                break;
            default:
                printf("Unknown instruction 0x%08x\n", inst);
                exit(1);
        }
    }

        
    inline uint32_t sext(uint32_t val, uint8_t t) {
        if(val >> t == 1)
            return ((1 << ((32 - t))) - 1) << t | val;
        return val;
    }

}