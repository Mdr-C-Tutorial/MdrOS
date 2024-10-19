#pragma once

#define KHEAP_INITIAL_SIZE 0xf00000
#define STACK_SIZE 32768
#define LONG_MAX 9223372036854775807L
#define LONG_MIN (-9223372036854775808L)

#define UINT32_MAX 0xffffffff
#define INT32_MAX 0x7fffffff

#define PADDING_DOWN(size, to) ((size_t)(size) / (size_t)(to) * (size_t)(to))
#define PADDING_UP(size, to)   PADDING_DOWN((size_t)(size) + (size_t)(to) - (size_t)1, to)

#define swap32(x)                                                              \
  ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >> 8) |                    \
   (((x) & 0x0000ff00) << 8) | (((x) & 0x000000ff) << 24))
#define swap16(x) ((((x) & 0xff00) >> 8) | (((x) & 0x00ff) << 8))

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
char *strstr(char *str1, char *str2);
char *strncpy(char *dest, const char *src, unsigned long long count);
char *strdup(const char *str);
void strtoupper(char *str);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
int isspace(int c);
int isdigit(int c);
int isalpha(int c);
int isupper(int c);
void sleep(uint32_t time);