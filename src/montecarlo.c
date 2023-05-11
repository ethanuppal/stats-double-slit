// Copyright (C) 2023 Ethan Uppal. All rights reserved.

#include "montecarlo.h"
#include <stdio.h> // printf
#include <stdlib.h> // malloc, realloc, free
#include <math.h> // sin, cos, M_PI
#include <time.h>

#define INITIAL_CAP 4
inline double_vector_t double_vector_new(void) {
    double_vector_t vec = malloc(sizeof(*vec) + INITIAL_CAP * sizeof(double));
    if (!vec) {
        return NULL;
    }
    vec->length = 0;
    vec->capacity = INITIAL_CAP;
    return vec;
}
inline void double_vector_push(double_vector_t* vec, double a, double b) {
    double_vector_t copy = *vec;
    if (copy->length + 2 >= copy->capacity) {
        copy->capacity *= 4;
        copy = realloc(copy, sizeof(*copy) + copy->capacity * sizeof(double));
    }
    copy->contents[copy->length++] = a;
    copy->contents[copy->length++] = b;
    *vec = copy;
}

inline double sinc(double x) {
    return (x == 0) ? (1.0) : (sin(x) / x);
}

// https://math.stackexchange.com/questions/2697259/fourier-transform-of-single-and-double-slit-functions
double norm_double_slit(double x, double slit_width, double slit_distance) {
    const double value = (cos(M_PI * slit_distance * x)
                          * sinc(slit_width * M_PI * x));
    return value * value;
}

simulation_t* sim_create(double* observed, size_t length, double range, double mean) {
    simulation_t* sim = malloc(sizeof(sim));
    sim->observed = observed;
    sim->length = length;
    sim->range = range;
    sim->mean = mean;
    if (!sim) {
        return NULL;
    }
    sim->distances = double_vector_new();
    if (!sim->distances) {
        return NULL;
    }
    sim->widths = double_vector_new();
    if (!sim->widths) {
        return NULL;
    }
    srand48(time(NULL));
    return sim;
}

void sim_free(simulation_t* sim) {
    free(sim->distances);
    free(sim->widths);
    free(sim);
}

void sim_trial(simulation_t* sim, double slit_width, double slit_distance) {
    double mean_squared_error = 0;
    for (size_t i = 0; i < sim->length; i++) {
        const double observed = sim->observed[i];
        const double observed_offset = (sim->range * (double)(i - 1)
                                        / (double)sim->length);
        const double observed_x = (observed_offset - sim->mean) / sim->range;
        const double expected = norm_double_slit(observed_x,
                                                 slit_width,
                                                 slit_distance);
        ;
        const double error = observed - expected;
        //printf("%f\n", expected);
        //fprintf(stderr, "%f %f\n", observed_x, expected);
        mean_squared_error += error * error;
    }
    double_vector_push(&sim->distances, slit_distance, mean_squared_error);
    double_vector_push(&sim->widths, slit_width, mean_squared_error);
}

void sim_results(const simulation_t* sim) {
    printf("Width\tMSE\t\n");
    double w, d, mse2 = INFINITY;
    for (size_t i = 0; i < sim->widths->length; i += 2) {
        double width = sim->widths->contents[i];
        double mse = sim->widths->contents[i + 1];
        printf("%f\t%f\n", width, mse);
        if (mse < mse2) {
            mse2 = mse;
            w = width;
        }
    }
    //printf("%f\t%f\n", w * sim->range, mse2);
    mse2 = INFINITY;
    printf("Distance\tMSE\t\n");
    for (size_t i = 0; i < sim->distances->length; i += 2) {
        double distance = sim->distances->contents[i];
        double mse = sim->distances->contents[i + 1];
        printf("%f\t%f\n", distance, mse);
        if (mse < mse2) {
            mse2 = mse;
            d = distance;
        }
    }
    //printf("%f\t%f\n", d * sim->range, mse2);
}
