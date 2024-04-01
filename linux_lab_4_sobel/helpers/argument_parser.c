//
// Created by ackbarca on 31.03.24.
//
#include "argument_parser.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

int parse_arguments(program_arguments *programArguments, char *argv[],
                    int argc) {
  if (argc < 3) {
    fprintf(
        stderr,
        "Invalid argument count! Using ./sobel <input_image> <output_image>");
    return -1;
  }

  programArguments->input_file = argv[optind];
  optind++;
  programArguments->output_file = argv[optind];
  optind++;

  int c = getopt(argc, argv, "+p:");
  switch (c) {
  case 'p':
    programArguments->pthread_count = atoi(optarg);
    break;
  case '?':
    fprintf(stderr, "Invalid options: %c", c);
    return -1;
  }

  return 0;
}