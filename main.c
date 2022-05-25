#include <stdio.h>
#include <stdlib.h>  // for EXIT_SUCCESS and EXIT_FAILURE
#include "bmp.h"
#include "bmp.c"

BMPImage *read_image(const char *filename, char **error);

void write_image(const char *filename, BMPImage *image, char **error);

FILE *open_file(const char *filename, const char *mode);

void handle_error(char **error, FILE *fp, BMPImage *image);

void clean_up(FILE *fp, BMPImage *image, char **error);

int main(void) {
    char *error = NULL;
    BMPImage *image = read_image("C:\\Users\\UFAZ\\CLionProjects\\ProjectBMPfile\\sample_640_426.bmp", &error);
    write_image("copy.bmp", image, &error);
    BMPImage *crop_image = crop_bmp(image, 2, 3, 4, 2, &error);
    write_image("crop.bmp", crop_image, &error);

    bool is_valid = check_bmp_header(&crop_image->header, fopen("crop.bmp", "rb"));

    clean_up(NULL, image, &error);
    clean_up(NULL, crop_image, &error);

    return EXIT_SUCCESS;
}

BMPImage *read_image(const char *filename, char **error) {
    FILE *input_ptr = open_file(filename, "rb");

    BMPImage *image = read_bmp(input_ptr, error);
    if (*error != NULL) {
        handle_error(error, input_ptr, image);
    }
    fclose(input_ptr);

    return image;
}

void write_image(const char *filename, BMPImage *image, char **error) {
    FILE *output_ptr = open_file(filename, "wb");

    if (!write_bmp(output_ptr, image, error)) {
        handle_error(error, output_ptr, image);
    }
    fclose(output_ptr);
}

/*
 * Open file. In case of error, print message and exit.
 */
FILE *open_file(const char *filename, const char *mode) {
    FILE *fp = fopen(filename, mode);
    if (fp == NULL) {
        fprintf(stderr, "Could not open file %s", filename);

        exit(EXIT_FAILURE);
    }

    return fp;
}

/*
 * Print error message and clean up resources.
 */
void handle_error(char **error, FILE *fp, BMPImage *image) {
    fprintf(stderr, "ERROR: %s\n", *error);
    clean_up(fp, image, error);

    exit(EXIT_FAILURE);
}

/*
 * Close file and release memory.
 */
void clean_up(FILE *fp, BMPImage *image, char **error) {
    if (fp != NULL) {
        fclose(fp);
    }
    free_bmp(image);
    free(*error);
}