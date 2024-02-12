#include "../inc/bus.hpp"
#include "../inc/def.hpp"

#include "../elf/inc/elf.hpp"

#include <fstream>

extern FILE *fp;

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
        uart = std::make_unique<uart_t>(trap);
        clint = std::make_unique<clint_t>(trap);
    }

    bus_t::~bus_t(){

    }

    void bus_t::step(){
        clint->step();
    }

    uint8_t bus_t::read8(uint32_t addr){
        if(addr > ramsize){
            printf("write8: addr: 0x%x\n", addr);
            exit(1);
        }
        switch(addr & 0xffff0000){
            case 0x10000000:
                return uart->read(addr);
            case 0x11000000:
                return clint->read(addr);
            default:
                return ram[addr];
        }
    }

    void bus_t::write8(uint32_t addr, uint8_t val){
        if(addr > ramsize){
            printf("write8: addr: 0x%x, val: 0x%x\n", addr, val);
            exit(1);
        }
        switch(addr & 0xffff0000){
            case 0x10000000:
                uart->write(addr, val);
                break;
            case 0x11000000:
                clint->write(addr, val);
                break;
            default:
                ram[addr] = val;
                break;
        }
        //fprintf(fp, "write8: addr: 0x%08x, val: 0x%02x\n", addr, val);
    }

    uint16_t bus_t::read16(bool trap_handle, uint32_t addr){
        if(trap_handle && (addr & 0x1) != 0){
            trap.set(
                trap_type::EXCEPTION,
                trap_code::LOAD_ADDR_MISALIGNED
            );
            printf("alignement error\n");
            exit(1);
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
            printf("alignement error\n");
            exit(1);
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
            printf("alignement error\n");
            exit(1);
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
            printf("alignement error\n");
            exit(1);
            return;
        }
        write8(addr, (uint8_t)(0xff & val));
        write8(addr + 1, (uint8_t)(0xff & (val >> 8)));
        write8(addr + 2, (uint8_t)(0xff & (val >> 16)));
        write8(addr + 3, (uint8_t)(0xff & (val >> 24)));
    }
}