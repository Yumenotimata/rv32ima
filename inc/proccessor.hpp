#pragma once

#include <memory>
#include <cstdlib>

#include "csr.hpp"
#include "disassembler.hpp"

namespace rv32ima{
    class register_t{
        public:
            register_t(){};
            ~register_t(){};
            uint32_t &operator[](int index){
                if(index < 0 || index > 31){
                    printf("Error: register index out of range %d\n", index);
                    exit(1);
                }
                reg[0] = 0x00000000; // x0 is hardwired to 0
                return reg[index];
            }
        private:
            uint32_t reg[32] = { 0x00000000 };
    };
    class bus_t;
    class processor_t{
        public:
            processor_t(std::unique_ptr<bus_t> &&bus);
            ~processor_t();

            void step();
            void fetch();
            void execute();

            csr_t csr;
            register_t reg;
            std::unique_ptr<bus_t> bus;
            uint32_t pc, inst; 
    };

    inline uint32_t sext(uint32_t val, uint8_t t);
}