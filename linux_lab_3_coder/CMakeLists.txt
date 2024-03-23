cmake_minimum_required(VERSION 3.27)
project(linux_lab_3_coder C)

set(CMAKE_C_STANDARD 11)

add_executable(linux_lab_3_reader
        reader/reader.c)

add_executable(linux_lab_3_coder
        coder/coder.c)
