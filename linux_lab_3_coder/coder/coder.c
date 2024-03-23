#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/param.h>

#define EXEC_FAILED -1
#define PATH_TO_READER "linux_lab_3_reader"
//
// Created by ackbarca on 23.03.24.
//
int exec_reader(char* path, int output_fd);
int xor_files(int first_fd, int second_fd, int output_fd);
void xor_arrays(const char* first_array, const char* second_array, char* output_array, int size);

int main(int argc, char* argv[]) {
    if(argc != 4) {
        fprintf(stderr, "Invalid argument count!");
        exit(1);
    }

    int output_fd = open(argv[3], O_WRONLY | O_CREAT);
    if(output_fd < 0) {
        perror("Output file error");
        exit(-1);
    }

    int first_reader_pipe[2];
    int second_reader_pipe[2];

    int err = pipe(first_reader_pipe);
    if(err < 0) {
        perror("Open pipe error");
        exit(-1);
    }

    err = pipe(second_reader_pipe);
    if(err < 0) {
        perror("Open pipe error");
        exit(-1);
    }

    err = exec_reader(argv[1], first_reader_pipe[1]);
    if(err < 0) {
        perror("Start reader error");
        exit(-1);
    }

    err = exec_reader(argv[2], second_reader_pipe[1]);
    if(err < 0) {
        perror("Start reader error");
        exit(-1);
    }

    xor_files(first_reader_pipe[0], second_reader_pipe[0], output_fd);

    close(first_reader_pipe[0]);
    close(second_reader_pipe[0]);
    close(output_fd);

    return 0;
}

int exec_reader(char* path, int output_fd) {
    int pid = fork();
    if(pid < 0)
        return EXEC_FAILED;

    if(pid == 0) {
        char fd_str[20];
        sprintf(fd_str, "%d", output_fd);
        int started = execl(PATH_TO_READER, PATH_TO_READER, path, fd_str);

        if(started < 0) {
            return -1;
        }
        return  -1;
    }
    close(output_fd);
    return 0;

}

int xor_files(int first_fd, int second_fd, int output_fd) {
    int read_bytes_first = 1;
    int read_bytes_second = 1;

    struct stat sbuf;
    if(fstat(first_fd, &sbuf)) {
        return -1;
    }

    int buf_size = MAX(sbuf.st_blksize, 1024);

    char* first_file_buffer = calloc(buf_size, sizeof(char));
    char* second_file_buffer = calloc(buf_size, sizeof(char));
    char* output_buffer = calloc(buf_size, sizeof(char));

    if(first_file_buffer == NULL || second_file_buffer == NULL || output_buffer == NULL)
        return -1;

    while (read_bytes_first != 0 || read_bytes_second != 0) {
        read_bytes_first = read(first_fd, first_file_buffer, buf_size);
        read_bytes_second = read(second_fd, second_file_buffer, buf_size);

        int read_size = MAX(read_bytes_second, read_bytes_first);

        xor_arrays(first_file_buffer, second_file_buffer, output_buffer, read_size);

        write(output_fd, output_buffer, read_size);
    }

    free(first_file_buffer);
    free(second_file_buffer);
    free(output_buffer);

    return 0;
}

void xor_arrays(const char* first_array, const char* second_array, char* output_array, int size) {
    for (int i = 0; i < size; ++i) {
        output_array[i] = first_array[i] ^ second_array[i];
    }
}