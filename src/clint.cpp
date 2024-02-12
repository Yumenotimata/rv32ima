#include "../inc/clint.hpp"
#include "../inc/def.hpp"

namespace rv32ima{
    clint_t::clint_t(trap_t &trap) 
        : trap(trap)
    {
        mtimecmp = 0;
        mtime = 0;
        msip = 0;
    }

    clint_t::~clint_t(){

    }

    void clint_t::step(){
        mtime ++;
        
        if(mtimecmp != 0 && mtime >= mtimecmp){
            trap.set(
                trap_type::INTERRUPT,
                trap_code::MACHINE_TIMER_INTERRUPT
            );    
        }

        if(msip != 0){
            trap.set(
                trap_type::INTERRUPT,
                trap_code::MACHINE_SOFTWARE_INTERRUPT
            );
        }
    }

    uint32_t clint_t::read(uint32_t addr){
        addr &= 0xffff;
        if(addr >= 0x0000 && addr <= 0x0003){
            return reinterpret_cast<uint8_t *>(&msip)[addr];
        }else if(addr >= 0x4000 && addr <= 0x4007){
            return reinterpret_cast<uint8_t *>(&mtimecmp)[addr - 0x4000];
        }else if(addr >= 0xbff8 && addr <= 0xbfff){
            return reinterpret_cast<uint8_t *>(&mtime)[addr - 0xbff8];
        }else{
            printf("clint read: addr: 0x%x\n", addr);
            exit(1);
        }
    }

    void clint_t::write(uint32_t addr, uint32_t val){
        addr &= 0xffff;
        if(addr >= 0x0000 && addr <= 0x0003){
            reinterpret_cast<uint8_t *>(&msip)[addr] = val;
        }else if(addr >= 0x4000 && addr <= 0x4007){
            reinterpret_cast<uint8_t *>(&mtimecmp)[addr - 0x4000] = val;
        }else if(addr >= 0xbff8 && addr <= 0xbfff){
            reinterpret_cast<uint8_t *>(&mtime)[addr - 0xbff8] = val;
        }else{
            printf("clint write: addr: 0x%x, val: 0x%x\n", addr, val);
            exit(1);
        }
    }
}