#pragma once

#include "ctypes.h"
#include <stdarg.h>

int vsprintf(char *buf, const char *fmt, va_list args);

void memclean(char *s, int len);
void* memcpy(void* s, const void* ct, size_t n);
int memcmp(const void *a_, const void *b_, uint32_t size);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t num);
size_t strnlen(const char *s, size_t maxlen);
size_t strlen(const char *str);
int isspace(int c);
int isdigit(int c);
int isalpha(int c);
int isupper(int c);