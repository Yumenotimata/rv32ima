cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20) 
project (elf LANGUAGES CXX)

set(EXECUTABLE_OUTPUT_PATH build)
set(LIBRARY_OUTPUT_PATH build)

add_library(elf src/elf.cpp)