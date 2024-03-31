cmake_minimum_required(VERSION 3.27)
project(linux_lab_4_sobel C)

set(CMAKE_C_STANDARD 11)

add_executable(linux_lab_4_sobel main.c
        helpers/jpeg_helper.c
        helpers/argument_parser.c)

target_link_libraries(linux_lab_4_sobel jpeg m)
