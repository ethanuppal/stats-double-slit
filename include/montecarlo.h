// Copyright (C) 2023 Ethan Uppal. All rights reserved.

#pragma once

#include <stddef.h> // size_t

typedef struct {
    size_t length;
    size_t capacity;
    double contents[];
} *double_vector_t;

double_vector_t double_vector_new(void);
void double_vector_push(double_vector_t* vec, double a, double b);

/// Computes the unnormalized sinc function with sinc(0) = 1.
double sinc(double x);

/// Computes the Fourier transform of the given double slit experimental setup.
double norm_double_slit(double x, double slit_distance, double slit_width);

typedef struct {
    double* observed;
    size_t length;
    double range;
    double mean;
    double_vector_t distances;
    double_vector_t widths;
} simulation_t;

simulation_t* sim_create(double* observed, size_t length, double range, double mean);
void sim_free(simulation_t* sim);

void sim_trial(simulation_t* sim, double slit_width, double slit_distance);

void sim_results(const simulation_t* sim);
