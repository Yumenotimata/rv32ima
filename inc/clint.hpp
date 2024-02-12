#pragma once

#include <cstdint>
#include <cstdio>
#include <cstdlib>

namespace rv32ima{
    struct trap_t;
    class clint_t{
        public:
            clint_t(trap_t &trap);
            ~clint_t();
            uint32_t read(uint32_t addr);
            void write(uint32_t addr, uint32_t val);
            void step();
        private:
            trap_t &trap;
            uint64_t mtimecmp, mtime, msip;
    };
}