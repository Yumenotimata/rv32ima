#include "../inc/bus.hpp"
#include "../inc/def.hpp"

#include "../elf/inc/elf.hpp"

#include <fstream>

namespace rv32ima{
    void load_binary(const char *bin_path, std::unique_ptr<uint8_t[]> &ram, uint32_t offset_addr){
        std::ifstream ifs(bin_path, std::ios::binary);
        ifs.seekg(0, std::ios::end);
        size_t size = ifs.tellg();
        ifs.seekg(0);
        char *bin = new char[size];
        ifs.read(bin, size);
        for(int i = 0; i < size; i++) 
            ram[offset_addr + i] = (uint8_t)bin[i];
        delete[] bin;
    }

    void load_elf(const char *elf_path, std::unique_ptr<uint8_t[]> &ram, uint32_t offset_addr, uint32_t ramsize){
        elf32_parser_t elf32(elf_path);
        //elf32.readelf();
        elf32.load(ram.get(), ramsize, LOAD);
    }

    bus_t::bus_t(std::unique_ptr<uint8_t[]> &&ram, uint32_t ramsize) 
        : ram(std::move(ram)), ramsize(ramsize)
    {

    }

    bus_t::~bus_t(){

    }

    uint8_t bus_t::read8(uint32_t addr){
        return ram[addr];
    }

    void bus_t::write8(uint32_t addr, uint8_t val){
        ram[addr] = val;
    }

    uint16_t bus_t::read16(bool trap_handle, uint32_t addr){
        if(trap_handle && (addr & 0x1) != 0){
            trap.set(
                trap_type::EXCEPTION,
                trap_code::LOAD_ADDR_MISALIGNED
            );
            return 0x0000;
        }
        return (read8(addr + 1) << 8) | read8(addr);
    }

    uint32_t bus_t::read32(bool trap_handle, uint32_t addr){
        if(trap_handle && (addr & 0x3) != 0){
            trap.set(
                trap_type::EXCEPTION,
                trap_code::LOAD_ADDR_MISALIGNED
            );
            return 0x00000000;
        }
        return (read8(addr + 3) << 24) | (read8(addr + 2) << 16) | (read8(addr + 1) << 8) | read8(addr);
    }

    void bus_t::write16(bool trap_handle, uint32_t addr, uint16_t val){
        if(trap_handle && (addr & 0x1) != 0){
            trap.set(
                trap_type::EXCEPTION,
                trap_code::STORE_ADDR_MISALIGNED
            );
            return;
        }
        write8(addr, (uint8_t)(0xff & val));
        write8(addr + 1, (uint8_t)(0xff & (val >> 8)));
    }

    void bus_t::write32(bool trap_handle, uint32_t addr, uint32_t val){
        if(trap_handle && (addr & 0x3) != 0){
            trap.set(
                trap_type::EXCEPTION,
                trap_code::STORE_ADDR_MISALIGNED
            );
            return;
        }
        write8(addr, (uint8_t)(0xff & val));
        write8(addr + 1, (uint8_t)(0xff & (val >> 8)));
        write8(addr + 2, (uint8_t)(0xff & (val >> 16)));
        write8(addr + 3, (uint8_t)(0xff & (val >> 24)));
    }
}