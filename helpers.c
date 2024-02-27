//
// Created by ackbarca on 22.02.24.
//
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <err.h>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>
#include "helpers.h"

long get_file_size(char *path_to_file) {
    FILE* file = fopen(path_to_file, "rb");
    if(file == NULL) {
        return -1;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    fclose(file);
    return file_size;
}
int create_directory(const char *path) {
    char *token, *path_copy;
    char delimiter[] = "/";
    char current_path[4097] = ".";

    path_copy = strdup(path);

    token = strtok(path_copy, delimiter);

    while (token != NULL) {
        strcat(current_path, "/");
        strcat(current_path, token);

        if (mkdir(current_path, S_IRWXU | S_IRWXG) == -1) {
            if (errno != EEXIST) {
                err(1, "%s", path_copy);
            }
        }

        token = strtok(NULL, delimiter);
    }

    free(path_copy);
    return 0;
}

int fd_open_file(char* path) {
    int file_flags;
    if(access(path, F_OK)) file_flags = O_CREAT | S_IWRITE | S_IREAD | O_WRONLY;
    else file_flags = S_IWRITE | S_IREAD  | O_WRONLY;
    int output_descriptor = open(path, file_flags);
    return output_descriptor;
}