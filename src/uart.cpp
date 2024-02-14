#include "../inc/uart.hpp"

namespace rv32ima{
    uart_t::uart_t(trap_t &trap)
        : trap(trap)
    {
    }

    void uart_t::write(uint32_t addr, uint8_t data){
        switch(addr & 0xff){
            case 0x00:
                printf("%c", data);
                break;
            default:
                //printf("unimplemented uart write 0x%x\n", addr);
                //exit(1);
                break;
        }
    }

    uint8_t uart_t::read(uint32_t addr){
        switch(addr & 0xff){
            case 0x00:
                return 0;
            case 0x05:
                return 0x60;
            default:
             //   printf("unimplemented uart read 0x%x\n", addr);
                return 0;
                break;
        }
    }
}