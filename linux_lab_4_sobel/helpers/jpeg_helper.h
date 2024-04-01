//
// Created by ackbarca on 31.03.24.
//

#ifndef LINUX_LAB_4_SOBEL_JPEG_HELPER_H
#define LINUX_LAB_4_SOBEL_JPEG_HELPER_H
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
typedef struct {
  unsigned int width;
  unsigned int height;
  unsigned char **imageData;
} decompressed_jpeg;
decompressed_jpeg applySobelFilter(decompressed_jpeg *image,
                                   unsigned int processedHeightStart,
                                   unsigned int processedHeightEnd);
void writeJPEG(const char *filename,
               const decompressed_jpeg *decompressedImage);
int readJPEG(FILE *infile, decompressed_jpeg *decompressedImage);
#endif // LINUX_LAB_4_SOBEL_JPEG_HELPER_H
