//
// Created by ackbarca on 31.03.24.
//
#include "jpeg_helper.h"
decompressed_jpeg applySobelFilter(decompressed_jpeg *image, unsigned int processedHeightStart, unsigned int processedHeightEnd) {
    int sobelX[3][3] = {{-1, 0, 1},
                        {-2, 0, 2},
                        {-1, 0, 1}};
    int sobelY[3][3] = {{-1, -2, -1},
                        {0, 0, 0},
                        {1, 2, 1}};
    decompressed_jpeg decompressedJpegResutlt;
    decompressedJpegResutlt.height = processedHeightEnd - processedHeightStart;
    decompressedJpegResutlt.width = image->width;
    decompressedJpegResutlt.imageData = (unsigned char**)malloc((processedHeightEnd - processedHeightStart) * sizeof(unsigned char*));

    for (int i = processedHeightStart, s = 0; i < processedHeightEnd; i++, s++) {
        decompressedJpegResutlt.imageData[s] = (unsigned char*)malloc(image->width * 3);
        for (int j = 0; j < image->width; j += 1) {
            int sumX = 0, sumY = 0;
            for (int k = -1; k <= 1; k++) {
                for (int l = -1; l <= 1; l++) {
                    int x = i + k;
                    int y = j + l;
                    if (x >= 0 && x < image->height && y >= 0 && y < image->width) {
                        sumX += image->imageData[x][y * 3] * sobelX[k + 1][l + 1];
                        sumY += image->imageData[x][y * 3] * sobelY[k + 1][l + 1];
                    }
                }
            }
            int magnitude = abs(sumX) + abs(sumY);
            magnitude = magnitude > 255 ? 255 : magnitude;
            decompressedJpegResutlt.imageData[s][j * 3] = decompressedJpegResutlt.imageData[s][j * 3 + 1] = decompressedJpegResutlt.imageData[s][j * 3 + 2] = magnitude;
        }
    }

    for (int i = processedHeightStart, s = 0; i < processedHeightEnd; i++, s++) {
        for (int j = 0; j < image->width * 3; j++) {
            image->imageData[i][j] = decompressedJpegResutlt.imageData[s][j];
        }
        free(decompressedJpegResutlt.imageData[s]);
    }
    free(decompressedJpegResutlt.imageData);

    return decompressedJpegResutlt;
}

void writeJPEG(const char *filename, const decompressed_jpeg *decompressedImage) {
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE* outfile;
    JSAMPROW row_pointer[1];
    int row_stride;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    if ((outfile = fopen(filename, "wb")) == NULL) {
        fprintf(stderr, "Can't open %s\n", filename);
        return;
    }

    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = decompressedImage->width;
    cinfo.image_height = decompressedImage->height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);

    jpeg_start_compress(&cinfo, TRUE);

    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = decompressedImage->imageData[cinfo.next_scanline];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
}

int readJPEG(FILE *infile, decompressed_jpeg *decompressedImage) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPARRAY buffer;
    unsigned int rowStride;


    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    (void) jpeg_read_header(&cinfo, TRUE);
    (void) jpeg_start_decompress(&cinfo);

    decompressedImage->width = cinfo.output_width;
    decompressedImage->height = cinfo.output_height;

    decompressedImage->imageData = (unsigned char **) malloc((decompressedImage->height) * sizeof(unsigned char *));
    for (int i = 0; i < decompressedImage->height; i++) {
        (decompressedImage->imageData)[i] = (unsigned char *) malloc((decompressedImage->width) * 3);
    }

    rowStride = decompressedImage->width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, rowStride, 1);

    while (cinfo.output_scanline < cinfo.output_height) {
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);
        unsigned int current_line = cinfo.output_scanline - 1;
        for (int i = 0; i < rowStride; i++) {
            (decompressedImage->imageData)[current_line][i] = buffer[0][i];
        }
    }

    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return 0;
}