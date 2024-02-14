#include "../inc/clint.hpp"
#include "../inc/def.hpp"
#include "../inc/csr.hpp"

namespace rv32ima{
    clint_t::clint_t(std::shared_ptr<csr_t> csr) 
        : csr(csr)
    {
        mtimecmp = 0;
        mtime = 0;
        msip = 0;
    }

    clint_t::~clint_t(){

    }

    void clint_t::step(){
        mtime ++;
        
        // if timer interrupt is enable
        if(static_cast<uint32_t>(csr->mstatus.ie) == 1 && 
            static_cast<uint32_t>(csr->mie.tie) == 1){
            if(mtimecmp != 0 && mtime >= mtimecmp){
                csr->set_trap_pending(
                    trap_type::INTERRUPT,
                    trap_code::MACHINE_TIMER_INTERRUPT
                );  
                csr->mip.tip = 1;
            }else{
                csr->mip.tip = 0;
            }
        }else{
            //csr->mip.tip = 0;
        }

        // if machine software interrupt is enable
        if(static_cast<uint32_t>(csr->mie.sie) == 1){
            if(msip != 0){
                csr->set_trap_pending(
                    trap_type::INTERRUPT,
                    trap_code::MACHINE_SOFTWARE_INTERRUPT
                );
                printf("machine software interrupt\n");
                exit(1);  
            }
        }
    }

    uint32_t clint_t::read(uint32_t addr){
        uint32_t _addr = addr & 0xffff;
        if(_addr >= 0x0000 && _addr <= 0x0003){
            return reinterpret_cast<uint8_t *>(&msip)[_addr];
        }else if(_addr >= 0x4000 && _addr <= 0x4007){
            return reinterpret_cast<uint8_t *>(&mtimecmp)[_addr - 0x4000];
        }else if(_addr >= 0xbff8 && _addr <= 0xbfff){
            return reinterpret_cast<uint8_t *>(&mtime)[_addr - 0xbff8];
        }else{
            printf("clint read: _addr: 0x%x\n", _addr);
            exit(1);
        }
    }

    void clint_t::write(uint32_t addr, uint8_t val){
        uint32_t _addr = addr & 0xffff;
         printf("clint write: _addr: 0x%x, val: 0x%x\n", _addr, val);
        if(_addr >= 0x0000 && _addr <= 0x0003){
            reinterpret_cast<uint8_t *>(&msip)[_addr] = val;
        }else if(_addr >= 0x4000 && _addr <= 0x4007){
            reinterpret_cast<uint8_t *>(&mtimecmp)[_addr - 0x4000] = val;
        }else if(_addr >= 0xbff8 && _addr <= 0xbfff){
            reinterpret_cast<uint8_t *>(&mtime)[_addr - 0xbff8] = val;
        }else{
            printf("clint write: _addr: 0x%x, val: 0x%x\n", _addr, val);
            exit(1);
        }
    }
}