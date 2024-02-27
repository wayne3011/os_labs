cmake_minimum_required(VERSION 3.27)
project(linux_lab_1_ar C)

set(CMAKE_C_FLAGS  "-Wall -Werror -Wextra")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER gcc)

add_executable(linux_lab_1_ar main.c
        argument_parser.c
        argument_parser.h
        archive_handler.h
        archive_handler.c
        helpers.h
        helpers.c
        extract_handler.h
        extract_handler.c)
