#include "helpers/argument_parser.h"
#include "helpers/jpeg_helper.h"
#include <math.h>
#include <pthread.h>
#include <string.h>
typedef struct {
  decompressed_jpeg *image;
  unsigned int processedHeightStart;
  unsigned int processedHeightEnd;
} sobel_routine_args;
void applySobelFilterMultitasking(decompressed_jpeg *decompressedImage,
                                  program_arguments args);
void *applySobelFilterRoutine(void *decompressedImage);

int main(int argc, char *argv[]) {
  program_arguments args;
  int err = parse_arguments(&args, argv, argc);
  if (err) {
    exit(1);
  }

  FILE *infile;
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

void applySobelFilterMultitasking(decompressed_jpeg *decompressedImage,
                                  program_arguments args) {
  pthread_t *threads =
      (pthread_t *)calloc(args.pthread_count, sizeof(pthread_t));
  sobel_routine_args *routines_args = (sobel_routine_args *)calloc(
      args.pthread_count, sizeof(sobel_routine_args));
  unsigned int step =
      floor((double)decompressedImage->height / args.pthread_count);
  for (int i = 0; i < args.pthread_count - 1; ++i) {
    routines_args[i].image = decompressedImage;
    routines_args[i].processedHeightStart = step * i;
    routines_args[i].processedHeightEnd = step * (i + 1);
    pthread_create(threads + i, NULL, applySobelFilterRoutine,
                   routines_args + i);
  }
  routines_args[args.pthread_count - 1].image = decompressedImage;
  routines_args[args.pthread_count - 1].processedHeightStart =
      step * (args.pthread_count - 1);
  routines_args[args.pthread_count - 1].processedHeightEnd =
      decompressedImage->height;
  pthread_create(threads + args.pthread_count - 1, NULL,
                 applySobelFilterRoutine,
                 routines_args + args.pthread_count - 1);

  sobel_routine_args **resultsImage = (sobel_routine_args **)calloc(
      args.pthread_count, sizeof(sobel_routine_args *));
  for (int i = 0; i < args.pthread_count; ++i) {
    pthread_join(threads[i], (void **)resultsImage + i);
  }

  for (int i = 0; i < args.pthread_count; ++i) {
    memmove(decompressedImage->imageData +
                resultsImage[i]->processedHeightStart,
            resultsImage[i]->image->imageData, step * sizeof(unsigned char *));
    free(resultsImage[i]->image->imageData);
    free(resultsImage[i]->image);
    free(resultsImage[i]);
  }
  free(resultsImage);
  free(threads);
  free(routines_args);
};

void *applySobelFilterRoutine(void *args) {
  sobel_routine_args *routine_args = args;
  decompressed_jpeg *resultJpeg =
      (decompressed_jpeg *)calloc(1, sizeof(decompressed_jpeg));
  *resultJpeg =
      applySobelFilter(routine_args->image, routine_args->processedHeightStart,
                       routine_args->processedHeightEnd);
  sobel_routine_args *result = calloc(1, sizeof(sobel_routine_args));
  result->processedHeightStart = routine_args->processedHeightStart;
  result->processedHeightEnd = routine_args->processedHeightEnd;
  result->image = resultJpeg;
  return result;
}