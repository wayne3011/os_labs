//
// Created by ackbarca on 22.02.24.
//
#include "extract_handler.h"
#include "helpers.h"
#include <dirent.h>
#include <err.h>
#include <fcntl.h>
#include <malloc.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <unistd.h>
int extract_handler(char *path_to_archive, char *path_to_output_folder) {
  printf("Extracting files...");
  int input_descriptor = open(path_to_archive, O_RDONLY);
  if (input_descriptor < 0) {
    err(1, "%s", path_to_archive);
  }
  while (process_file(input_descriptor, path_to_output_folder) == 0)
    ;
  close(input_descriptor);
  printf("Done!");
  return 0;
}

int process_file(int input_descriptor, char *path_to_output_folder) {
  long file_size = 0;
  int n = read(input_descriptor, &file_size, sizeof(long));
  if (n != sizeof(long)) {
    return CORRUPTED_FILE;
  }

  char delim = 0;
  read(input_descriptor, &delim, sizeof(char));
  if (delim != ' ')
    return CORRUPTED_FILE;

  char file_name[MAX_FILE_PATH_LENGTH + 1] = {0};
  if (read_filename(input_descriptor, file_name))
    return CORRUPTED_FILE;

  char folder_path[MAX_FILE_PATH_LENGTH + 1] = {0};
  char *p_folder_path = folder_path;
  if (path_to_output_folder != NULL) {
    strcat(p_folder_path, path_to_output_folder);
    size_t len = strlen(p_folder_path);
    if (p_folder_path[len - 1] != '/') {
      p_folder_path[len] = '/';
      p_folder_path[len + 1] = '\0';
      len++;
    }
    p_folder_path += len;
  }
  strcat(p_folder_path, file_name);
  char *last_slash = strrchr(p_folder_path, '/');
  if (last_slash != NULL) {
    int last_slash_index = last_slash - folder_path;
    folder_path[last_slash_index] = '\0';
    create_directory(folder_path);
    folder_path[last_slash_index] = '/';
  }

  int fd = fd_open_file(folder_path);
  if (fd < 0) {
    err(1, "%s", folder_path);
  }
  int err = extract_file(fd, input_descriptor, file_size);
  close(fd);

  read(input_descriptor, &delim, sizeof(char));
  if (delim != ' ')
    return err = CORRUPTED_FILE;
  return err;
}

int read_filename(int input_descriptor, char *file_name) {
  char ch = 0;
  while (ch != ' ') {
    int n = read(input_descriptor, &ch, sizeof(char));
    if (n == -1)
      return CORRUPTED_FILE;
    *file_name = ch;
    file_name++;
  }
  *file_name = '\0';
  return 0;
}

int extract_file(int output_descriptor, int input_descriptor, size_t size) {
  static struct stat sbuf;
  if (fstat(output_descriptor, &sbuf)) {
    return -1;
  }
  int bsize = MAX(sbuf.st_blksize, 1024);

  int batch_count = size / bsize;

  char *buf = calloc(bsize, sizeof(char));

  if (buf == NULL)
    return -1;

  ssize_t nr = 0, nw = 0;

  while (batch_count > 0 && (nr = read(input_descriptor, buf, bsize)) > 0) {
    for (int off = 0; nr > 0 && nw != -1; nr -= nw, off += nw) {
      nw = write(output_descriptor, buf + off, (size_t)nr);
    }
    batch_count--;
  }
  nr = read(input_descriptor, buf, size % bsize);
  for (int off = 0; nr > 0 && nw != -1; nr -= nw, off += nw) {
    nw = write(output_descriptor, buf + off, (size_t)nr);
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
