#include "../inc/proccessor.hpp"
#include "../inc/bus.hpp"

#include <unistd.h>
#include <fstream>
#include <iostream>
#include <signal.h>

bool sig = true;
void sigint_handler(int signum){
    sig = false;
    printf("SIGINT received\n");
}

FILE *fp;

int main(int argc, char** argv){
    signal(SIGINT, sigint_handler);

    std::unique_ptr<uint8_t[]> ram((uint8_t *)calloc(0xffffffff, sizeof(uint8_t)));

    rv32ima::load_binary(argv[1], ram, 0x80000000);
    rv32ima::load_binary(argv[2], ram, 0x80000000 + 0x3FFFA00);
    std::unique_ptr<rv32ima::bus_t> bus(new rv32ima::bus_t(std::move(ram), 0xffffffff));
    rv32ima::processor_t processor(std::move(bus));

    processor.reg[11] = 0x80000000 + 0x3FFFA00;


    while(sig){
        processor.step();
    }

    return 0;
}