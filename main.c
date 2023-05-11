// Copyright (C) 2023 Ethan Uppal. All rights reserved.

#include <stdio.h> // FILE, fopen, fclose, perror, EOF
#include <stdlib.h> // calloc, malloc, free
#include "pgm.h" // pgm_t, pgm_load, pgm_free
#include "analyze.h" // pix_aggregate_t, pgm_vertical_reduce
#include "montecarlo.h" // simulation_t, sim_create, sim_free

#define NTRIALS 10000
#define RANGE 6500 // micrometers

int main() {
    // Read in pgm file
    FILE* file = fopen("data/data.pgm", "r");
    if (!file) {
        perror("fread");
        return 1;
    }
    pgm_t* pgm = pgm_load(file);
    if (!pgm) {
        perror("pgm_load");
        return 1;
    }
    if (fclose(file) == EOF) {
        perror("fclose");
        return 1;
    }

    pgm_header_dump(pgm);

    // Perform vertical reduction and create buckets
    pix_aggregate_t* buckets = calloc(pgm->width, sizeof(*buckets));
    if (!buckets) {
        fprintf(stderr, "calloc: Virtual memory exhausted.\n");
        return 1;
    }
    pgm_vertical_reduce(buckets, pgm);

    // Find max bucket value for scaling and total bucket sum for normalizing
    pix_aggregate_t bucket_max = 0;
    pix_aggregate_t bucket_sum = 0;
    for (size_t i = 0; i < pgm->width; i++) {
        bucket_sum += buckets[i];
        if (buckets[i] > bucket_max) {
            bucket_max = buckets[i];
        }
    }

    // Convert buckets to scaled buckets and find mean
    double* scaled_buckets = malloc(pgm->width * sizeof(*scaled_buckets));
    double mean = 0;
    for (size_t i = 0; i < pgm->width; i++) {
        scaled_buckets[i] = (double)(buckets[i]) / (double)bucket_max;
        mean += (double)i * (double)(buckets[i]);
    }
    mean *= (double)RANGE / ((double)(pgm->width - 1) * bucket_sum);
    free(buckets);

    // Run the simulation
    simulation_t* sim = sim_create(scaled_buckets, pgm->width, RANGE, mean);
    for (size_t i = 0; i < NTRIALS; i++) {
        double sim_distance = (double)20 * drand48() / 2.0;
        double sim_width = sim_distance * drand48() / 2.0; // bounded by half
        sim_trial(sim, sim_width, sim_distance);
    }
    //sim_trial(sim, 100.0, 500.0);
    //sim_trial(sim, 2.311613, 9.393214);
    sim_results(sim);

    // Discard simulation object and scaled buckets
    sim_free(sim);
    free(scaled_buckets);

    // Free resources associated with pgm file
    pgm_free(pgm);

    fprintf(stderr, "%f\n", norm_double_slit(0.482852, 1, 5));

    return 0;
}
