#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

typedef int ssize_t;
typedef size_t usize;
typedef ssize_t isize;

#ifdef __cplusplus
extern "C" {
#endif

static inline int isdigit(int c) {
    return (c >= '0' && c <= '9');
}

static inline int isalpha(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static inline int isalnum(int c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9')) return 8;
    return 0;
}

static inline int islower(int c) {
    if (c >= 'a' && c <= 'z') return 512;
    return 0;
}

static inline int ispunct(char ch) {
    return ch == '!' || ch == '@' || ch == '#' || ch == '$' || ch == '%' ||
           ch == '^' || ch == '&' || ch == '*' || ch == '(' || ch == ')' ||
           ch == '+' || ch == '-' || ch == '/' || ch == '?' || ch == '"' ||
           ch == '\'' || ch == ':' || ch == ';' || ch == '{' || ch == '[' ||
           ch == ']' || ch == '}' || ch == '|' || ch == '\\' || ch == '~' ||
           ch == '`' || ch == '<' || ch == '>' || ch == ',' || ch == '.';
}

static inline char toupper(char ch) { return (ch & 0xDF); }

static inline char tolower(char ch) { return (ch | 0x20); }

static inline int iscsymf(int ch) {
    return ch == '_' || isdigit(ch) || isalpha(ch);
}

static inline int isascll(char ch) { return ((ch & 0x80) == 0); }

static inline int iscntrl(char ch) {
    return ((ch == 0x7F) || (ch >= 0x00 && ch <= 0x1F));
}

static inline int isprint(int c) { return (c > 0x1F && c < 0x7F); }

static inline int isgraph(int c) { return (c > 0x20 && c < 0x7F); }

static inline int isxdigit(int c) {
    if (!((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) {
        if (!isdigit(c))
            return 0;
    }
    return 1;
}

static inline int isspace(int c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' ||
            c == '\v');
}

static inline int isupper(int c) { return (c >= 'A' && c <= 'Z'); }

#ifdef __cplusplus
}
#endif
