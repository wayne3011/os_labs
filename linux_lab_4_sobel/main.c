#include <pthread.h>
#include "helpers/jpeg_helper.h"
#include "helpers/argument_parser.h"
#include <math.h>
#include <string.h>
typedef struct {
    decompressed_jpeg* image;
    unsigned int processedHeightStart;
    unsigned int processedHeightEnd;
} sobel_routine_args;
void applySobelFilterMultitasking(decompressed_jpeg* decompressedImage, program_arguments args);
void* applySobelFilterRoutine(void* decompressedImage);

int main(int argc, char* argv[]) {
    program_arguments args;
    int err = parse_arguments(&args, argv, argc);
    if(err) {
        exit(1);
    }

    FILE* infile;
    if ((infile = fopen(args.input_file, "rb")) == NULL) {
        fprintf(stderr, "Can't open %s\n", args.input_file);
        return 1;
    }

    decompressed_jpeg decompressedImage;
    readJPEG(infile, &decompressedImage);

    applySobelFilterMultitasking(&decompressedImage, args);

    writeJPEG(args.output_file, &decompressedImage);

    for (int i = 0; i < decompressedImage.height; i++) {
        free(decompressedImage.imageData[i]);
    }
    free(decompressedImage.imageData);

    return 0;
}

void applySobelFilterMultitasking(decompressed_jpeg* decompressedImage, program_arguments args) {
    pthread_t* threads = (pthread_t*)calloc(args.pthread_count, sizeof (pthread_t));
    sobel_routine_args* routines_args = (sobel_routine_args*) calloc(args.pthread_count, sizeof(sobel_routine_args));
    unsigned int step = floor((double)decompressedImage->height / args.pthread_count);
    for (int i = 0; i < args.pthread_count - 1; ++i) {
        routines_args[i].image = decompressedImage;
        routines_args[i].processedHeightStart = step * i;
        routines_args[i].processedHeightEnd = step * (i+1);
        pthread_create(threads + i, NULL, applySobelFilterRoutine, routines_args + i);
    }
    routines_args[args.pthread_count - 1].image = decompressedImage;
    routines_args[args.pthread_count - 1].processedHeightStart = step * (args.pthread_count - 1);
    routines_args[args.pthread_count - 1].processedHeightEnd = decompressedImage->height;
    pthread_create(threads + args.pthread_count - 1, NULL, applySobelFilterRoutine, routines_args + args.pthread_count - 1);

    for (int i = 0; i < args.pthread_count; ++i) {
        void* thread_result;
        pthread_join(threads[i], &thread_result);
    }
    free(threads);
    free(routines_args);
};

void* applySobelFilterRoutine(void* args) {
    sobel_routine_args* routine_args = args;
    applySobelFilter(routine_args->image, routine_args->processedHeightStart, routine_args->processedHeightEnd);
}