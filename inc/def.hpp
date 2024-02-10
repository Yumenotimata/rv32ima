#pragma once

#include <cstdint>

namespace rv32ima{
    template <typename T, uint8_t e,uint8_t s = e>
    class bitfield {
        public:
            static constexpr T mask = ((T)(1 << (e - s + 1)) - 1) << s;
            void operator=(const T val) {raw = (raw & ~mask) | (((mask >> s) & val) << s);}
            explicit operator T() const {return ((raw & mask) >> s);}
        private:
            T raw;
    };

    enum struct trap_code_t{
        LOAD_ADDR_MISALIGNED    = 4,
        LOAD_ACCESS_FAULT       = 5,
        STORE_ADDR_MISALIGNED   = 6,
        STORE_ACCESS_FAULT      = 7,
        ECALL_FROM_MMODE        = 11,
    };

    enum struct trap_type_t{
        EXCEPTION = 0,
        INTERRUPT = 1
    };

    struct trap_t{
        public:
            void set(trap_type_t type, trap_code_t code){
                this->type = type;
                this->code = code;
                this->valid = true;
            };
            void disable(){
                this->valid = false;
            };
            bool is_valid(){
                return valid;
            };
        private:
            trap_type_t type;
            trap_code_t code;
            bool valid;
    };
}