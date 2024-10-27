#pragma once

#define KERNEL_NAME "CP_Kernel-i386-0.4.0"

#define KHEAP_INITIAL_SIZE 0xf00000
#define STACK_SIZE 32768
#define USER_AREA_START 0x90000000

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

typedef struct _xstr {
    size_t len;
    size_t hash;
    char   data[];
} *xstr;

int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);
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
char *strchrnul(const char *s, int c);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
int isspace(int c);
int isdigit(int c);
int isalpha(int c);
int isupper(int c);
void sleep(uint32_t time);

#define streq(s1, s2)                                                                              \
  ({                                                                                               \
    char* _s1 = (s1), *_s2 = (s2);                                                                 \
    (_s1 && _s2) ? strcmp(_s1, _s2) == 0 : _s1 == _s2;                                             \
  })

#define strneq(s1, s2, n)                                                                          \
  ({                                                                                               \
    char* _s1 = (s1), *_s2 = (s2);                                                                 \
    (_s1 && _s2) ? strncmp(_s1, _s2, n) == 0 : _s1 == _s2;                                         \
  })

#define xstreq(s1, s2)                                                                             \
  ({                                                                                               \
    xstr _s1 = (s1), *_s2 = (s2);                                                                  \
    (_s1 && _s2) ? (_s1->hash == _s2->hash ? xstrcmp(_s1, _s2) == 0 : false) : _s1 == _s2;         \
  })

#define memeq(s1, s2, n)                                                                           \
  ({                                                                                               \
    char* _s1 = (s1), *_s2 = (s2);                                                                 \
    (_s1 && _s2) ? memcmp(_s1, _s2, n) == 0 : _s1 == _s2;                                          \
  })