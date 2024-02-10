#include "../inc/elf.hpp"

elf32_parser_t::elf32_parser_t(const char *elf_path) {

    fp = fopen(elf_path,"rb");
    if(fp == NULL){
		printf("No such file or directory \"%s\".\n",elf_path);
		exit(1);
	}

	fseek(fp,0,SEEK_END);
	size_t elf_size = ftell(fp);
	fseek(fp,0,SEEK_SET);

    elf_buf = (uint8_t *)calloc(elf_size,sizeof(uint8_t));

	fread(elf_buf,1,elf_size,fp);

    header = (elf32_header_t *)elf_buf;
    for(int i = 0;i < header->e_phnum; i++) 
        p_headers.push_back((elf32_program_header_t *)(elf_buf + header->e_phoff + sizeof(elf32_program_header_t) * i));
}


/**
 * @brief show some details for only ELF header and program header.
*/
void elf32_parser_t::readelf() {
    printf("ELF Header:\n");
    printf(" ELF Header:\n");
    printf("\tEntry point address:\t\t0x%08x\n",header->e_entry);
    printf("\tSize of this header:\t\t%d (bytes)\n",header->e_ehsize);
    printf("\tSize of program headers:\t%d (bytes)\n",header->e_pehtsize);
    printf("\tStart of program headers:\t%d (bytes into file)\n",header->e_phoff);
    printf("\tNumber of program headers:\t%d\n",header->e_phnum);

    printf("\n");
    printf("There are %ld program headers, starting at offset %d\n\n",p_headers.size(),header->e_phoff);
    printf(" Program Headers:\n");
    printf("\tType\tOffset\t\tVirtAddr\tPhysAddr\tFileSiz\tMemSiz\tAlign\n");
    for(const auto& e: p_headers) 
        printf("\t%s\t0x%06x\t0x%08x\t0x%08x\t0x%05x\t0x%x\t0x%x\n",e->p_type == 1 ? "LOAD" : "UNKNOWN",e->p_offset,e->p_vaddr,e->p_paddr,e->filesz,e->p_memmsz,e->p_align);
}

/**
 * @brief load segment that is shown in the program header to ram.
 * @param[ram] head reference of ram you want to load segment. 
 * @param[segment_type] integer value of the attributes of you want to load.
*/
void elf32_parser_t::load(uint8_t *ram, size_t ram_size, uint32_t segment_type) {
    for(const auto& e: p_headers) {
        if(e->p_type == segment_type) {
            if(e->p_paddr >= ram_size || (e->p_paddr + e->filesz) >= ram_size) {
                printf("segment physical addr is over than ram size. please relocate segment.\n");
                exit(1);
            }
            //printf("\nLoad segment '%d' to 0x%08x-0x%08x\n",e->p_type,e->p_paddr,e->p_paddr + e->filesz);
            for(uint32_t offset = 0; offset < e->filesz; offset++) 
                ram[e->p_paddr + offset] = *(uint8_t *)(elf_buf + e->p_offset + offset);
        }
    }
}

void elf32_parser_t::load(std::vector<uint8_t> &ram, size_t ram_size, uint32_t segment_type) {
    for(const auto& e: p_headers) {
        if(e->p_type == segment_type) {
            if(e->p_paddr >= ram_size || (e->p_paddr + e->filesz) >= ram_size) {
                printf("segment physical addr is over than ram size. please relocate segment.\n");
                exit(1);
            }
            // printf("\nLoad segment '%d' to 0x%08x-0x%08x\n",e->p_type,e->p_paddr,e->p_paddr + e->filesz);
            for(uint32_t offset = 0; offset < e->filesz; offset++) 
                ram[e->p_paddr + offset] = *(uint8_t *)(elf_buf + e->p_offset + offset);
        }
    }
}

elf32_parser_t::~elf32_parser_t() {
    fclose(fp);
}