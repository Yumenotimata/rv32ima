cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20) 
project (rv32ima LANGUAGES CXX)

set(EXECUTABLE_OUTPUT_PATH build)
set(LIBRARY_OUTPUT_PATH build)

add_subdirectory(elf)

add_executable(main src/_main.cpp src/processor.cpp src/disassembler.cpp src/bus.cpp src/csr.cpp)
target_link_libraries(main elf)