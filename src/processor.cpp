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

    void processor_t::handle_trap(){
        csr.mcause.trap_type = bus->trap.type;
        csr.mcause.trap_code = bus->trap.code;
        csr.mstatus.pie = (uint32_t)csr.mstatus.ie;
        csr.mepc = pc;

        if((uint32_t)csr.mstatus.ie == 1 && bus->trap.type == trap_type::INTERRUPT){
            if(bus->trap.code == trap_code::MACHINE_TIMER_INTERRUPT) csr.mip.tip = 1;
        }

        csr.mstatus.ie = 0;

        if((uint32_t)csr.mtvec.mode == trap_vector_mode::VECTORED){
            pc = ((uint32_t)csr.mtvec.base << 2) + (bus->trap.code << 2);
        }else if((uint32_t)csr.mtvec.mode == trap_vector_mode::DIRECT){
            pc = ((uint32_t)csr.mtvec.base << 2);
        }
    }

    void processor_t::step(){
        if(bus->trap.has_occurred()){
            handle_trap();
            bus->trap.clear();
        }

        fetch();
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
                if(funct7 == 0x1)
                {
                    switch(funct3)
                    {
                        case 0x0: // mul
                            reg[rd] = (int32_t)reg[rs1] * (int32_t)reg[rs2]; break;
                        case 0x1: // mulh
                            reg[rd] = (int32_t)(((int64_t)(int32_t)reg[rs1] * (int64_t)(int32_t)reg[rs2]) >> 32); break;
                        case 0x2: // mulhsu
                            reg[rd] = (int32_t)(((int64_t)(int32_t)reg[rs1] * (uint64_t)(uint32_t)reg[rs2]) >> 32); break;
                        case 0x3: // mulhu
                            reg[rd] = (uint32_t)(((uint64_t)(uint32_t)reg[rs1] * (uint64_t)(uint32_t)reg[rs2]) >> 32); break;
                        case 0x4: // div
                            if(reg[rs2] == 0) reg[rd] = -1; else if(reg[rs2] == -1) reg[rd] = reg[rd]; else reg[rd] = (int32_t)reg[rs1] / (int32_t)reg[rs2]; break;
                        case 0x5: // divu
                            if(reg[rs2] == 0) reg[rd] = -1; else reg[rd] = (uint32_t)reg[rs1] / (uint32_t)reg[rs2]; break;
                        case 0x6: // rem
                            if(reg[rs2] == 0) reg[rd] = reg[rs1]; else if(reg[rs2] == -1) reg[rd] = 0; else reg[rd] = (int32_t)reg[rs1] % (int32_t)reg[rs2]; break;
                        case 0x7: // remu
                            if(reg[rs2] == 0) reg[rd] = reg[rs1]; else reg[rd] = (uint32_t)reg[rs1] % (uint32_t)reg[rs2]; break;
                    }
                }else
                {
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
                    case 0x0: // system
                        switch(funct7)
                        {
                            case 0x0: // ecall
                                bus->trap.set(trap_type::EXCEPTION, trap_code::ECALL_FROM_MMODE); break;
                            case 0x18: // mret
                                pc = csr.mepc - 4; break;
                            default:
                                printf("Unknown system instruction 0x%08x\n", inst);
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
            case 0x2f:
                switch(funct5)
                {
                    case 0x1: // amoswap.w
                        reg[rd] = bus->read32(true, reg[rs1]); bus->write32(true, reg[rs1], reg[rs2]); break;
                    case 0x0: // amoadd.w
                        reg[rd] = bus->read32(true, reg[rs1]); bus->write32(true, reg[rs1], reg[rs2] + reg[rd]); break;
                    case 0x8: // amoor.w
                        reg[rd] = bus->read32(true, reg[rs1]); bus->write32(true, reg[rs1], reg[rs2] | reg[rd]); break;
                    case 0xc: // amoand.w
                        reg[rd] = bus->read32(true, reg[rs1]); bus->write32(true, reg[rs1], reg[rs2] & reg[rd]); break;
                    case 0x2: // lr.w
                        reg[rd] = bus->read32(true, reg[rs1]); register_reservation[reg[rs1]] = true; break;
                    case 0x3: // sc.w
                    {
                        auto it = register_reservation.find(reg[rs1]);
                        if(it != register_reservation.end()) {
                            register_reservation.erase(it);
                            bus->write32(true, reg[rs1], reg[rs2]);
                            reg[rd] = 0;
                        }else {
                            reg[rd] = 1;
                        } 
                        break;
                    }
                    default:
                        printf("Unknown amo instruction 0x%08x, pc = 0x%08x\n", inst, pc);
                        exit(1);
                }
                break;
            default:
                printf("Unknown instruction 0x%08x, pc = 0x%08x\n", inst, pc);
                exit(1);
        }
    }

        
    inline uint32_t sext(uint32_t val, uint8_t t) {
        if(val >> t == 1)
            return ((1 << ((32 - t))) - 1) << t | val;
        return val;
    }

}