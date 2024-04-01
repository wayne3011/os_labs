//
// Created by ackbarca on 31.03.24.
//

#ifndef LINUX_LAB_4_SOBEL_ARGUMENT_PARSER_H
#define LINUX_LAB_4_SOBEL_ARGUMENT_PARSER_H
typedef struct {
  char *input_file;
  char *output_file;
  int pthread_count;
} program_arguments;
int parse_arguments(program_arguments *programArguments, char *argv[],
                    int argc);
#endif // LINUX_LAB_4_SOBEL_ARGUMENT_PARSER_H
