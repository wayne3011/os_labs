cmake_minimum_required(VERSION 3.27)
project(linux_lab_2_bash C)

set(CMAKE_C_STANDARD 11)

add_executable(linux_lab_2_bash main.c
        helpers.h
        helpers.c
        list.h
        list.c)
