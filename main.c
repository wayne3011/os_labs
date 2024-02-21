#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <err.h>
#include <sys/param.h>
#include <unistd.h>
#include "argument_parser.h"

int dir_bypass(char *root_dir, char *next_dir, int output_descriptor);
long get_file_size(char *path_to_file);
int archive_folder(char *folder_name, int output_descriptor);
int archive_file(int output_descriptor, int input_descriptor);
int main(int argc, char* argv[]) {
    Flags flags = {0};
    char* file_names;
    int error = parse_arguments(argc, argv, &flags, &file_names);
    char output_path[4097] = {0};
    strcat(output_path, "./");
    strcat(output_path, file_names);
    strcat(output_path, ".s304");

    if(error == 0) {
        printf("a: %d\n", flags.flag_a);
        printf("e: %d\n", flags.flag_e);
        printf("path: %s\n", file_names);
    }
    int file_flags;
    if(access(output_path, F_OK)) file_flags = O_CREAT | S_IWRITE | S_IREAD | O_WRONLY;
    else file_flags = S_IWRITE | S_IREAD  | O_WRONLY;
    int output_descriptor = open(output_path, file_flags);
    if(output_descriptor < 0) {
        err(1, "%s", file_names);
    }
    dir_bypass(file_names, "", output_descriptor);
    close(output_descriptor);
}

int dir_bypass(char *root_dir, char *next_dir, int output_descriptor) {
    char current_path[4097] = {0};
    strcat(current_path, root_dir);
    strcat(current_path, next_dir);
    strcat(current_path, "/");

    DIR* current_dir = opendir(current_path);
    if(current_dir == NULL)
        return 0;
    archive_folder(current_path, output_descriptor);
    struct dirent* sub_dir;
    while((sub_dir = readdir(current_dir)) != NULL) {
        if(sub_dir->d_type == DT_DIR && strcmp(sub_dir->d_name, ".") != 0
                                        && strcmp(sub_dir->d_name, "..") != 0) {
            char current_path_dir[4097] = "";
            strcat(current_path_dir, current_path);
            strcat(current_path_dir, sub_dir->d_name);
            dir_bypass(current_path, sub_dir->d_name, output_descriptor);
        }
    }
    closedir(current_dir);
    return 0;
}

int archive_folder(char *folder_name, int output_descriptor) {
    struct dirent* sub_dir;
    DIR * folder = opendir(folder_name);
    if(folder == NULL) {
        err(1, "%s", folder_name);
    }
    char delimiter = ' ';
    while((sub_dir = readdir(folder)) != NULL) {
        if(sub_dir->d_type != DT_DIR && strcmp(sub_dir->d_name, ".") != 0
           && strcmp(sub_dir->d_name, "..") != 0) {
            char current_path_dir[4097] = "";
            strcat(current_path_dir, folder_name);
            strcat(current_path_dir, sub_dir->d_name);

            long file_size = get_file_size(current_path_dir);
            if(file_size == -1) {
                return -1;
            }

            write(output_descriptor, &file_size, sizeof (long ));
            write(output_descriptor, &delimiter, sizeof (char));
            write(output_descriptor, &current_path_dir, strlen(current_path_dir) * sizeof (char));
            write(output_descriptor, &delimiter, sizeof (char));

            int input_descriptor = open(current_path_dir, O_RDONLY);
            if(input_descriptor < 0){
                err(1, "%s", current_path_dir);
            }
            archive_file(output_descriptor, input_descriptor);
            write(output_descriptor, &delimiter, sizeof (char));
        }
    }
    closedir(folder);
    return 0;
}

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

int archive_file(int output_descriptor, int input_descriptor) {
    static struct stat sbuf;
    if(fstat(output_descriptor, &sbuf)) {
        return -1;
    }
    int bsize = MAX(sbuf.st_blksize, 1024);

    char* buf = calloc(bsize,sizeof(char));

    if(buf == NULL) return -1;

    ssize_t nr = 0, nw = 0;

    while((nr = read(input_descriptor, buf, bsize)) > 0){
        for (int off = 0; nr > 0 && nw != -1; nr -= nw, off += nw) {
            nw = write(output_descriptor, buf + off, (size_t)nr);
        }
    }
    if(nr == -1 || nw == -1) {
        err(1, "");
    };
    if(buf != NULL) {
        free(buf);
        buf = NULL;
    }
    return 0;
}
