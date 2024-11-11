#pragma once

#include <stddef.h>
#include "ctype.h"

void exit(int retval);
void *calloc(size_t nelem, size_t elsize);
void *malloc(unsigned size);
void *realloc(void *ptr, unsigned newsize);
void free(void *ptr);
void *xmalloc(size_t size);

static inline int atoi(const char **s) {
    int i = 0;

    while (isdigit(**s))
        i = i * 10 + *((*s)++) - '0';
    return i;
}

static inline void abort(void){
    exit(-1);
}