#pragma once

#include <cstdint>
#include <cstdio>

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

    using trap_code_t = uint32_t;
    namespace trap_code{
        constexpr trap_code_t LOAD_ADDR_MISALIGNED    = 4;
        constexpr trap_code_t LOAD_ACCESS_FAULT       = 5;
        constexpr trap_code_t STORE_ADDR_MISALIGNED   = 6;
        constexpr trap_code_t STORE_ACCESS_FAULT      = 7;
        constexpr trap_code_t ECALL_FROM_MMODE        = 11;
        constexpr trap_code_t MACHINE_TIMER_INTERRUPT = 0;
        constexpr trap_code_t MACHINE_SOFTWARE_INTERRUPT = 3;
    };

    using trap_type_t = uint32_t;
    namespace trap_type{
        constexpr trap_type_t EXCEPTION = 0;
        constexpr trap_type_t INTERRUPT = 1;
    };

    using trap_vector_mode_t = uint32_t;
    namespace trap_vector_mode{
        constexpr trap_vector_mode_t DIRECT     = 0;
        constexpr trap_vector_mode_t VECTORED   = 1;
    };

    struct trap_t{
        public:
            void set(trap_type_t type, trap_code_t code){
                if(occured && type != trap_type::EXCEPTION) return;
                this->type = type;
                this->code = code;
                this->occured = true;
            };
            void clear(){
                this->occured = false;
            };
            bool has_occurred(){
                return occured;
            };
            trap_type_t type;
            trap_code_t code;
        private:
            bool occured;
    };
}