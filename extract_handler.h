//
// Created by ackbarca on 22.02.24.
//

#ifndef LINUX_LAB_1_AR_EXTRACT_HANDLER_H
#define LINUX_LAB_1_AR_EXTRACT_HANDLER_H

#include <glob.h>

#define CORRUPTED_FILE (-1)
int extract_handler(char *path_to_archive, char *path_to_output_folder);
int read_filename(int input_descriptor, char* file_name);
int process_file(int input_descriptor, char *path_to_output_folder);
int extract_file(int output_descriptor, int input_descriptor, size_t size);
#endif //LINUX_LAB_1_AR_EXTRACT_HANDLER_H
