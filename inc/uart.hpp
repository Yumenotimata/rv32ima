#pragma once

#include <cstdint>
#include <cstdio>
#include <cstdlib>

namespace rv32ima{
    struct trap_t;
    class uart_t{
        public:
            uart_t(trap_t &trap);

            void write(uint32_t addr, uint8_t data);
            uint8_t read(uint32_t addr);
        private:
            trap_t &trap;
    };
}