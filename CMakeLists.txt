cmake_minimum_required(VERSION 3.27)
project(linux_lab_1_ar C)

set(CMAKE_C_STANDARD 11)

add_executable(linux_lab_1_ar main.c
        argument_parser.c
        argument_parser.h)
