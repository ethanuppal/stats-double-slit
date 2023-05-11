// Copyright (C) 2023 Ethan Uppal. All rights reserved.

#pragma once

#include <stddef.h> // size_t
#include "pgm.h" // pgm_t

typedef size_t pix_aggregate_t;

void pgm_vertical_reduce(pix_aggregate_t buckets[], pgm_t* pgm);
