#pragma once 

#include <cstdint>
#include <cstdlib>
#include <cstdio>

namespace rv32ima{
    constexpr uint32_t mtvec    = 0x305;
    constexpr uint32_t mepc     = 0x341;
    constexpr uint32_t mcause   = 0x342;
    class csr_t{
        public:
            csr_t(){};
            ~csr_t(){};

            uint32_t &operator[](int index){
                if(index < 0 || index > 4095){
                    printf("Error: csr index out of range %d\n", index);
                    exit(1);
                }
                return csr[index];
            }
        private:
            uint32_t csr[4096] = { 0x00000000 };
    };
}