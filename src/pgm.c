// Copyright (C) 2023 Ethan Uppal. All rights reserved.

#include "pgm.h" // pgm_t, size_t, FILE
#include <stdio.h> // fscanf, fread, EOF, fprintf
#include <stdlib.h> // malloc, free, NULL
#include <errno.h> // errno, EINVAL

pgm_t* pgm_load(FILE* file) {
    // Parse in file header
    size_t width, height, max_channel;
    char discard;
    int result = fscanf(file, "P5%c%zu%c%zu%c%zu%c", &discard, &width,
                        &discard, &height, &discard, &max_channel, &discard);
    if (result == EOF || result != 7 /* number of items scanned */) {
        errno = EINVAL;
        return NULL;
    }

    // Allocate a pgm object
    const size_t data_size = width * height;
    pgm_t* pgm = malloc(sizeof(*pgm) + data_size);
    if (!pgm) {
        return NULL;
    }
    pgm->width = width;
    pgm->height = height;

    // Read in data
    if (fread(pgm->contents, sizeof(char), data_size, file) != data_size) {
        return NULL;
    }

    return pgm;
}

inline void pgm_free(pgm_t* pgm) {
    // Just free it lol
    free(pgm);
}

void pgm_header_dump(pgm_t* pgm) {
    fprintf(stderr, "P5 width=%zu height=%zu\n", pgm->width, pgm->height);
}
