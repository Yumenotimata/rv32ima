#pragma once 

#include <cstdint>
#include <cstdlib>
#include <cstdio>

#include "def.hpp"

namespace rv32ima{
    constexpr uint32_t MTVEC    = 0x305;
    constexpr uint32_t MEPC     = 0x341;
    constexpr uint32_t MCAUSE   = 0x342;
    constexpr uint32_t MIE      = 0x304;
    constexpr uint32_t MIP      = 0x344;
    constexpr uint32_t MSTATUS  = 0x300;

    class csr_t{
        public:
            csr_t(){};
            ~csr_t(){};

            uint32_t &operator[](int index){
                if(index < 0 || index > 4095){
                    printf("Error: csr index out of range %d\n", index);
                    exit(1);
                }
                switch(index)
                {
                    case MCAUSE:    return mcause.raw;
                    case MTVEC:     return mtvec.raw;
                    case MIE:       return mie.raw;
                    case MIP:       return mip.raw;
                    case MSTATUS:   return mstatus.raw;
                    case MEPC:      return mepc;
                }
                return csr[index];
            }

            union {
                bitfield<uint32_t, 11>  eie;
                bitfield<uint32_t, 7>   tie;
                bitfield<uint32_t, 3>   sie;
                uint32_t raw;
                operator uint32_t() {return raw;}
                void operator=(uint32_t val) {raw = val;}
                bool is_any_interrupt_enabled(){
                    return (uint32_t)eie == 1 || (uint32_t)tie == 1 || (uint32_t)sie == 1;
                };
            }mie;

            union {
                bitfield<uint32_t, 11>  eip;
                bitfield<uint32_t, 7>   tip;
                bitfield<uint32_t, 3>   sip;
                uint32_t raw;
                operator uint32_t() {return raw;}
                void operator=(uint32_t val) {raw = val;}
                bool is_any_interrupt_pending(){
                    return (uint32_t)eip == 1 || (uint32_t)tip == 1 || (uint32_t)sip == 1;
                };
            }mip;

            union {
                bitfield<uint32_t, 12, 11>  pp;
                bitfield<uint32_t, 7>       pie;
                bitfield<uint32_t, 3>       ie;
                uint32_t raw;
                operator uint32_t() {return raw;}
                void operator=(uint32_t val) {raw = val;}
            }mstatus;

            union{
                bitfield<uint32_t, 31>      trap_type;
                bitfield<uint32_t, 30, 0>   trap_code; 
                uint32_t raw;
                operator uint32_t() {return raw;}
                void operator=(uint32_t val) {raw = val;}
            }mcause;

            union{
                bitfield<uint32_t, 31, 2>   base;
                bitfield<uint32_t, 1, 0>    mode;
                uint32_t raw;
                operator uint32_t() {return raw;}
                void operator=(uint32_t val) {raw = val;}
            }mtvec;

            uint32_t mepc;
        private:
            uint32_t csr[4096] = { 0x00000000 };
    };
}