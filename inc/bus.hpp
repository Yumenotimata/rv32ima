#pragma once

#include "../inc/def.hpp"
#include "../inc/uart.hpp"
#include "../inc/clint.hpp"
#include <memory>

namespace rv32ima {

    void load_binary(const char *bin_path, std::unique_ptr<uint8_t[]> &ram, uint32_t offset_addr);
    void load_elf(const char *elf_path, std::unique_ptr<uint8_t[]> &ram, uint32_t offset_addr, uint32_t ramsize);

    class bus_t {
    public:
        bus_t(std::unique_ptr<uint8_t[]> &&ram, uint32_t ramsize);
        ~bus_t();
        
        trap_t trap;
        std::unique_ptr<uart_t> uart;
        std::unique_ptr<clint_t> clint;
        std::unique_ptr<uint8_t[]> ram;
        uint32_t ramsize;

        void step();
        uint8_t read8(uint32_t addr);
        uint16_t read16(bool trap_handle, uint32_t addr);
        uint32_t read32(bool trap_handle, uint32_t addr);
        void write8(uint32_t addr, uint8_t val);
        void write16(bool trap_handle, uint32_t addr, uint16_t val);
        void write32(bool trap_handle, uint32_t addr, uint32_t val);
    };
}