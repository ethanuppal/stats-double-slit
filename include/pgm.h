// Copyright (C) 2023 Ethan Uppal. All rights reserved.

#pragma once

#include <stdio.h> // FILE
#include <stddef.h> // size_t

/// Represents a NetPGM file.
typedef struct {
    size_t width;
    size_t height;
    char contents[];
} pgm_t;

/// Loads pgm data from the given file, returning an allocated pgm object.
pgm_t* pgm_load(FILE* file);

/// Frees the resources associated with the given pgm object.
void pgm_free(pgm_t* pgm);

/// Prints metadata of the given pgm object.
void pgm_header_dump(pgm_t* pgm);
