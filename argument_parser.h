//
// Created by ackbarca on 21.02.24.
//

#ifndef LINUX_LAB_1_AR_ARGUMENT_PARSER_H
#define LINUX_LAB_1_AR_ARGUMENT_PARSER_H
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVALID_FLAG 1
#define PARSE_FAILED 2
typedef struct {
    bool flag_a;
    bool flag_e;
} Flags;
int parse_arguments(int argc, char **argv, Flags *flags, char *folder_path, char **output_file_path);
void help();
#endif //LINUX_LAB_1_AR_ARGUMENT_PARSER_H
