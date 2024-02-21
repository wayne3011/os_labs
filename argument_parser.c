#include "argument_parser.h"

//
// Created by ackbarca on 21.02.24.
//

int parse_arguments(int argc, char **argv, Flags *flags, char **folder_path)  {
    int c;
    int long_option_index = 0;


    static struct option long_opts[] = {{"archive", 0, 0, 'a'},
                                        {"extract", 0, 0, 'e'},
                                        {"help", 0, 0, 'h'},
                                        {0,         0, 0, 0}};
    c = getopt_long(argc, argv, "aeh", long_opts, &long_option_index);
    switch (c) {
        case 'a':
            flags->flag_a = true;
            break;
        case 'e':
            flags->flag_e = true;
            break;
        case 'h':
            help();
            return 0;
        case -1:
            break;
        case '?':
        default:
            printf("Invalid option: -%c\n", c);
            return PARSE_FAILED;
    }
    c = getopt_long(argc, argv, "aeh", long_opts, &long_option_index);
    if(c != -1) {
        help();
        return PARSE_FAILED;
    }
    if(optind == argc) {
        help();
        return PARSE_FAILED;
    }
    *folder_path = argv[optind];
    return 0;
}

void help() {
    printf("Usage: ar [a|e] [folder_path]\n");
}