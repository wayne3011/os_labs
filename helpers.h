//
// Created by ackbarca on 22.02.24.
//

#ifndef LINUX_LAB_1_AR_HELPERS_H
#define LINUX_LAB_1_AR_HELPERS_H
#define MAX_FILE_PATH_LENGTH 4096

long get_file_size(char *path_to_file);
int create_directory(const char *path);
int fd_open_file(char *path);
#endif // LINUX_LAB_1_AR_HELPERS_H
