//
// Created by ackbarca on 22.02.24.
//
#include "archive_handler.h"
#include "helpers.h"
#include <err.h>
#include <fcntl.h>
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <unistd.h>

int archive_handler(char *root_dir) {
  printf("Archiving the file...");
  char output_path[MAX_FILE_PATH_LENGTH + 1] = {0};

  strcat(output_path, "./");
  int len = strlen(root_dir);
  if (root_dir[len - 1] != '/') {
    len--;
    root_dir[len - 1] = '\0';
  }
  char *last_slash = strrchr(root_dir, '/');
  if (last_slash != NULL)
    root_dir[last_slash - root_dir] = '\0';
  strcat(output_path, root_dir);
  strcat(output_path, ".s304");

  int output_descriptor = fd_open_file(output_path);
  if (output_descriptor < 0) {
    err(1, "%s", output_path);
  }
  dir_bypass(root_dir, output_descriptor);
  close(output_descriptor);
  printf("Done!");
  return 0;
}

int dir_bypass(char *root_dir, int output_descriptor) {

  DIR *current_dir = opendir(root_dir);
  if (current_dir == NULL) {
    err(1, "%s", root_dir);
  }
  archive_folder(root_dir, output_descriptor);
  struct dirent *sub_dir;
  size_t root_path_length = strlen(root_dir);
  while ((sub_dir = readdir(current_dir)) != NULL) {
    if (sub_dir->d_type == DT_DIR && strcmp(sub_dir->d_name, ".") != 0 &&
        strcmp(sub_dir->d_name, "..") != 0) {
      strcat(root_dir, "/");
      strcat(root_dir, sub_dir->d_name);
      dir_bypass(root_dir, output_descriptor);
      root_dir[root_path_length] = '\0';
    }
  }
  closedir(current_dir);
  return 0;
}

int archive_folder(char *folder_name, int output_descriptor) {
  struct dirent *sub_dir;
  DIR *folder = opendir(folder_name);
  if (folder == NULL) {
    err(1, "%s", folder_name);
  }
  char delimiter = ' ';
  size_t root_folder_name_length = strlen(folder_name);
  while ((sub_dir = readdir(folder)) != NULL) {
    if (sub_dir->d_type != DT_DIR && strcmp(sub_dir->d_name, ".") != 0 &&
        strcmp(sub_dir->d_name, "..") != 0) {

      strcat(folder_name, "/");
      strcat(folder_name, sub_dir->d_name);

      long file_size = get_file_size(folder_name);
      if (file_size == -1) {
        err(1, "%s", folder_name);
      }

      write(output_descriptor, &file_size, sizeof(long));
      write(output_descriptor, &delimiter, sizeof(char));
      write(output_descriptor, folder_name, strlen(folder_name) * sizeof(char));
      write(output_descriptor, &delimiter, sizeof(char));

      int input_descriptor = open(folder_name, O_RDONLY);
      if (input_descriptor < 0) {
        err(1, "%s", folder_name);
      }

      archive_file(output_descriptor, input_descriptor);
      write(output_descriptor, &delimiter, sizeof(char));
      folder_name[root_folder_name_length] = '\0';
    }
  }
  closedir(folder);
  return 0;
}

int archive_file(int output_descriptor, int input_descriptor) {
  static struct stat sbuf;
  if (fstat(output_descriptor, &sbuf)) {
    return -1;
  }
  int bsize = MAX(sbuf.st_blksize, 1024);

  char *buf = calloc(bsize, sizeof(char));

  if (buf == NULL)
    return -1;

  ssize_t nr = 0, nw = 0;

  while ((nr = read(input_descriptor, buf, bsize)) > 0) {
    for (int off = 0; nr > 0 && nw != -1; nr -= nw, off += nw) {
      nw = write(output_descriptor, buf + off, (size_t)nr);
    }
  }
  if (nr == -1 || nw == -1) {
    err(1, "While archive file:");
  };
  if (buf != NULL) {
    free(buf);
    buf = NULL;
  }
  return 0;
}
