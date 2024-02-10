#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <stdlib.h>
#include <vector>

// Segment Attributes
#define LOAD 1

#define EI_INDENT 16
typedef struct {
    uint8_t e_indet[EI_INDENT];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_pehtsize;
    uint16_t e_phnum;
    uint16_t e_shetsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
}elf32_header_t;

typedef struct {
    uint32_t p_type;
    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t filesz;
    uint32_t p_memmsz;
    uint32_t p_flags;
    uint32_t p_align;
}elf32_program_header_t;

class elf32_parser_t {
    public:
        elf32_parser_t(const char *elf_path);
        ~elf32_parser_t();

        size_t elf_size;
        uint8_t *elf_buf;
        FILE *fp;
        elf32_header_t *header;
        std::vector<elf32_program_header_t *> p_headers;

        void readelf();
        void load(uint8_t *ram, size_t ram_size, uint32_t segment_type);
        void load(std::vector<uint8_t> &ram, size_t ram_size, uint32_t segment_type);
};

class elf32_loader_t {

};