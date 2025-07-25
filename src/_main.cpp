#include "../inc/proccessor.hpp"
#include "../inc/bus.hpp"

#include <iostream>
#include <vector>
#include <unistd.h>

FILE *fp;

int main(void){
    std::vector<char *> test_entry = {
        "bin/rv32mi-p-csr",
        "bin/rv32ui-p-sub",
        "bin/rv32ui-p-add",
        "bin/rv32ui-p-addi",
        "bin/rv32ui-p-and",
        "bin/rv32ui-p-andi",
        "bin/rv32ui-p-auipc",
        "bin/rv32ui-p-beq",
        "bin/rv32ui-p-bge",
        "bin/rv32ui-p-bgeu",
        "bin/rv32ui-p-blt",
        "bin/rv32ui-p-bltu",
        "bin/rv32ui-p-bne",
        "bin/rv32ui-p-fence_i",
        "bin/rv32ui-p-jal",
        "bin/rv32ui-p-jalr",
        "bin/rv32ui-p-lb",
        "bin/rv32ui-p-lbu",
        "bin/rv32ui-p-lh",
        "bin/rv32ui-p-lhu",
        "bin/rv32ui-p-lui",
        "bin/rv32ui-p-lw",
        "bin/rv32ui-p-or",
        "bin/rv32ui-p-ori",
        "bin/rv32ui-p-sb",
        "bin/rv32ui-p-sh",
        "bin/rv32ui-p-sll",
        "bin/rv32ui-p-slli",
        "bin/rv32ui-p-slt",
        "bin/rv32ui-p-slti",
        "bin/rv32ui-p-sltiu",
        "bin/rv32ui-p-sltu",
        "bin/rv32ui-p-sra",
        "bin/rv32ui-p-srai",
        "bin/rv32ui-p-srl",
        "bin/rv32ui-p-srli",
        "bin/rv32ui-p-sub",
        "bin/rv32ui-p-sw",
        "bin/rv32ui-p-xor",
        "bin/rv32ui-p-xori",
        "bin/rv32ua-p-amoadd_w",
        "bin/rv32ua-p-amoor_w",
        "bin/rv32ua-p-amoand_w",
        "bin/rv32ua-p-amoswap_w",
        "bin/rv32ua-p-lrsc",
        "bin/rv32um-p-div",
        "bin/rv32um-p-mul",
        "bin/rv32um-p-mulh",
        "bin/rv32um-p-rem",
        "bin/rv32um-p-remu"
    };

    for(int i = 0; i < test_entry.size(); i++) {
        printf("test \"%s\"\t\t",test_entry[i]);
        std::unique_ptr<uint8_t[]> ram(new uint8_t[0xffffffff]);
        rv32ima::load_elf(test_entry[i], ram, 0x80000000, 0xffffffff);
        std::unique_ptr<rv32ima::bus_t> bus(new rv32ima::bus_t(std::move(ram), 0xffffffff));
        rv32ima::processor_t processor(std::move(bus));

        while(true){
            processor.step();
            if(processor.pc == 0x80000044) {
                if(processor.reg[3] == 1)
                    std::cout << "test passed" << std::endl;
                else
                    std::cout << "test failed" << std::endl;
                break;
            }
            usleep(10000);
        }
    }

    return 0;
}
