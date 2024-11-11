#pragma once

#include "ctype.h"

int printf(const char *format,...);
int putchar(char c);
int puts(char *string);
int vsnprintf(char *buf, size_t n, const char *fmt, va_list ap);
int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);
