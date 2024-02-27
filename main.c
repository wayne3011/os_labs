#include "argument_parser.h"
#include "archive_handler.h"
#include "helpers.h"
#include "extract_handler.h"
int main(int argc, char* argv[]) {
    Flags flags = {0};
    char path[MAX_FILE_PATH_LENGTH + 1] = {0};
    char* output_path;
    int error = parse_arguments(argc, argv, &flags, (char *) (path), &output_path);

    if(error == 0) {
        printf("a: %d\n", flags.flag_a);
        printf("e: %d\n", flags.flag_e);
        printf("path: %s\n", path);
    }

    if(flags.flag_a) archive_handler(path);
    else if(flags.flag_e) extract_handler(path, output_path);
}


