#pragma once

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>

namespace rv32ima{
    class csr_t;
    class clint_t{
        public:
            clint_t(std::shared_ptr<csr_t> csr);
            ~clint_t();
            uint32_t read(uint32_t addr);
            void write(uint32_t addr, uint32_t val);
            void step();
        private:
            std::shared_ptr<csr_t> csr;
            uint64_t mtimecmp, mtime, msip;
    };
}