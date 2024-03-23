#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <err.h>
#include <sys/param.h>
#include <unistd.h>
#include <fcntl.h>
int print_file(int output_fd, int input_fd);

//
// Created by ackbarca on 23.03.24.
//
int main(int argc, char* argv[]) {
    //
    FILE* log_fd = fopen("log", "w");
    //
    stdout = log_fd;
    stderr = log_fd;

    int output_fd = 0;
    if(argc == 2) output_fd = fileno(stdout);
    else {
        char* c_ptr;
        output_fd = strtol(argv[2],&c_ptr, 10);

        if(*c_ptr != '\0') {
            fprintf(stderr, "Invalid first argument!");
            close(output_fd);
            exit(1);
        }

    }

    int input_fd = open(argv[1], O_RDONLY);

    if(input_fd < 0) {
        perror("Input path error");
    }
    else {
        print_file(output_fd, input_fd);
    }
    fclose(log_fd);
    close(output_fd);
    close(input_fd);
}

int print_file(int output_fd, int input_fd) {
    static struct stat sbuf;
    if (fstat(output_fd, &sbuf)) {
        return -1;
    }
    int bsize = MAX(sbuf.st_blksize, 1024);

    char *buf = calloc(bsize, sizeof(char));

    if (buf == NULL) {
        fprintf(stderr, "Allocation error!");
        exit(1);
    }

    ssize_t nr = 0, nw = 0;

    while ((nr = read(input_fd, buf, bsize)) > 0) {
        for (int off = 0; nr > 0 && nw != -1; nr -= nw, off += nw) {
            nw = write(output_fd, buf + off, (size_t)nr);
        }
    }
    if (nr == -1 || nw == -1) {
        err(1, "While archive file:");
    }

    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }
    return 0;
}
