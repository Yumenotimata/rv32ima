#include "../inc/proccessor.hpp"
#include "../inc/bus.hpp"

#include <unistd.h>

int main(int argc, char** argv){
    std::unique_ptr<uint8_t[]> ram(new uint8_t[0xffffffff]);
    rv32ima::load_binary(argv[1], ram, 0x80000000);
    std::unique_ptr<rv32ima::bus_t> bus(new rv32ima::bus_t(std::move(ram), 0xffffffff));
    rv32ima::processor_t processor(std::move(bus));

    while(true){
        processor.step();
        printf(".\n");
        usleep(10000000);
    }

    return 0;
}