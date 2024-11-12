#pragma once

#include "ctype.h"

#define MAX_PRECISION (10)

// String length functions
size_t strlen(const char *str);
size_t strnlen(const char *s, size_t maxlen);

// String comparison functions
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
int strcoll(const char *str1, const char *str2);

// String copy functions
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, unsigned long long count);

// String concatenation functions
char *strcat(char *dest, const char *src);
char *strncat(char *dest, const char *src, unsigned long long count);

// Memory functions
void *memcpy(void *s, const void *ct, size_t n);
int memcmp(const void *a_, const void *b_, uint32_t size);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t num);

// String search functions
const char *strstr(const char *str1, const char *str2);
char *strchr(const char *s, const char ch);
char *strpbrk(const char *str, const char *strCharSet);
size_t strspn(const char *string, const char *control);

// String utility functions
char *strdup(const char *str);
double strtod(const char *nptr, char **endptr);
char *ftoa(double f, char *buf, int precision);
