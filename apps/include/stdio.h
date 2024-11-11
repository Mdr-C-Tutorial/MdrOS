#pragma once

#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>

int printf(char *format,...);
int putchar(char c);
int puts(char *string);
int vsnprintf(char *buf, size_t n, const char *fmt, va_list ap);
int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);
