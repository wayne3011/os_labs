//
// Created by ackbarca on 22.02.24.
//

#ifndef LINUX_LAB_1_AR_ARCHIVE_HANDLER_H
#define LINUX_LAB_1_AR_ARCHIVE_HANDLER_H
int dir_bypass(char *root_dir, int output_descriptor);
int archive_handler(char* root_dir);

int archive_folder(char *folder_name, int output_descriptor);
int archive_file(int output_descriptor, int input_descriptor);
#endif //LINUX_LAB_1_AR_ARCHIVE_HANDLER_H
