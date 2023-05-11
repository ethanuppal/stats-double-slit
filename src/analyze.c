// Copyright (C) 2023 Ethan Uppal. All rights reserved.

#include "analyze.h" // pix_aggregate_t, pgm_t

void pgm_vertical_reduce(pix_aggregate_t buckets[], pgm_t* pgm) {
    // Accumulate into buckets row-by-row for efficiency.
    size_t i = 0;
    size_t bucket_index;
    for (size_t y = 0; y < pgm->height; y++) {
        bucket_index = 0;
        for (size_t x = 0; x < pgm->width; x++) {
            // Accumulate into bucket.
            buckets[bucket_index] += (pix_aggregate_t)pgm->contents[i];
            i++;
            bucket_index++;
        }
    }
}
