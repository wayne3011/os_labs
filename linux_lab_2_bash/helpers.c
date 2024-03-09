//
// Created by ackbarca on 08.03.24.
//
#include "helpers.h"

int strchrc(const char *str, char symbol) {
    int counter = 0;
    while (*str != '\0') {
        if(*str == symbol) counter++;
        str++;
    }
    return counter;
}

