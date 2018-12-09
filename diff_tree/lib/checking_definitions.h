#pragma once

#define ALLOC_CHECK(pointer) { \
    if (pointer == NULL) { \
        fprintf(stderr, "\033[31mError:\033[0m Allocation failed\n"); \
        exit(1); \
    } \
}

