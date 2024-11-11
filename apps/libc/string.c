#include "string.h"
#include "stdlib.h"

static const double rounders[MAX_PRECISION + 1] = {
    0.5,          // 0
    0.05,         // 1
    0.005,        // 2
    0.0005,       // 3
    0.00005,      // 4
    0.000005,     // 5
    0.0000005,    // 6
    0.00000005,   // 7
    0.000000005,  // 8
    0.0000000005, // 9
    0.00000000005 // 10
};

size_t strnlen(const char *s, size_t maxlen) {
    const char *es = s;
    while (*es && maxlen) {
        es++;
        maxlen--;
    }

    return (es - s);
}

size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

int strcmp(const char *s1, const char *s2) {
    char is_equal = 1;

    for (; (*s1 != '\0') && (*s2 != '\0'); s1++, s2++) {
        if (*s1 != *s2) {
            is_equal = 0;
            break;
        }
    }

    if (is_equal) {
        if (*s1 != '\0') {
            return 1;
        } else if (*s2 != '\0') {
            return -1;
        } else {
            return 0;
        }
    } else {
        return (int)(*s1 - *s2);
    }
}

char *strcpy(char *dest, const char *src) {
    do {
        *dest++ = *src++;
    } while (*src != 0);
    *dest = 0;
}

char *strcat(char *dest, const char *src) {
    char *temp = dest;
    while (*temp != '\0')
        temp++;
    while ((*temp++ = *src++) != '\0')
        ;
}

void *memcpy(void *s, const void *ct, size_t n) {
    if (NULL == s || NULL == ct || n <= 0)
        return NULL;
    while (n--)
        *(char *)s++ = *(char *)ct++;
    return s;
}

int memcmp(const void *a_, const void *b_, uint32_t size) {
    const char *a = a_;
    const char *b = b_;
    while (size-- > 0) {
        if (*a != *b)
            return *a > *b ? 1 : -1;
        a++, b++;
    }
    return 0;
}

void *memset(void *s, int c, size_t n) {
    unsigned char *p = s;
    while (n-- > 0)
        *p++ = c;
    return s;
}

void *memmove(void *dest, const void *src, size_t num) {
    void *ret = dest;
    if (dest < src) {
        while (num--) // 前->后
        {
            *(char *)dest = *(char *)src;
            dest = (char *)dest + 1;
            src = (char *)src + 1;
        }
    } else {
        while (num--) // 后->前
        {
            *((char *)dest + num) = *((char *)src + num);
        }
    }
    return ret;
}

char *strncat(char *dest, const char *src, unsigned long long count) {
    if (dest == 0 || src == 0)
        return 0;
    char *ret = dest;
    while (*dest != '\0')
        dest++;
    for (unsigned long long i = 0; i < count; ++i)
        dest[i] = src[i];
    dest[count] = '\0';
    return ret;
}

char *strncpy(char *dest, const char *src, unsigned long long count) {
    if (dest == 0 || src == 0)
        return 0;
    char *ret = dest;
    while (count)
        *dest = *src, ++dest, ++src, --count;
    return ret;
}

const char *strstr(const char *str1, const char *str2) {
    if (str1 == 0 || str2 == 0)
        return 0;
    const char *temp = 0;
    const char *res = 0;
    while (*str1 != '\0') {
        temp = str1;
        res = str2;
        while (*temp == *res)
            ++temp, ++res;
        if (*res == '\0')
            return str1;
        ++str1;
    }
    return 0;
}

char *strdup(const char *str) {
    if (str == NULL)
        return NULL;

    char *strat = (char *)str;
    int len = 0;
    while (*str++ != '\0')
        len++;
    char *ret = (char *)malloc(len + 1);

    while ((*ret++ = *strat++) != '\0') {
    }

    return ret - (len + 1);
}

char *strchr(const char *s, const char ch) {
    if (NULL == s)
        return NULL;
    const char *pSrc = s;
    while ('\0' != *pSrc) {
        if (*pSrc == ch) {
            return (char *)pSrc;
        }
        ++pSrc;
    }
    return NULL;
}

size_t strspn(const char *string, const char *control) {
    const char *str = (const char *)string;
    const char *ctrl = (const char *)control;
    unsigned char map[32];
    int count = 0;
    /*clear the map*/
    memset(map, 0, 32 * sizeof(unsigned char));
    // memset(map,0,32);
    /*set bits in control map*/
    while (*ctrl) {
        map[*ctrl >> 3] |= (0x01 << (*ctrl & 7));
        ctrl++;
    }
    /*count the str's char num in control*/
    if (*str) {
        count = 0;
        while ((map[*str >> 3] & (0x01 << (*str & 7)))) {
            count++;
            str++;
        }
        return count;
    }
    return 0;
}

char *strpbrk(const char *str, const char *strCharSet) {
    while (*str) {
        const char *pSet = strCharSet;
        while (*pSet)
            if (*pSet++ == *str)
                return (char *)str;
        ++str;
    }
    return NULL;
}

int strcoll(const char *str1, const char *str2) {
    return strcmp(str1, str2);
}

double strtod(const char *nptr, char **endptr) {
    double number;
    int exponent;
    int negative;
    char *p = (char *)nptr;
    double p10;
    int n;
    int num_digits;
    int num_decimals;

    /* Skip leading whitespace */
    while (isspace(*p))
        p++;

    /* Handle optional sign */
    negative = 0;
    switch (*p) {
    case '-':
        negative = 1;
        p++;
        break;
    case '+':
        p++;
        break;
    }

    number = 0.;
    exponent = 0;
    num_digits = 0;
    num_decimals = 0;

    /* Process string of digits */
    while (isdigit(*p)) {
        number = number * 10. + (*p - '0');
        p++;
        num_digits++;
    }

    /* Process decimal part */
    if (*p == '.') {
        p++;

        while (isdigit(*p)) {
            number = number * 10. + (*p - '0');
            p++;
            num_digits++;
            num_decimals++;
        }

        exponent -= num_decimals;
    }

    if (num_digits == 0) {
        return 0.0;
    }

    /* Correct for sign */
    if (negative)
        number = -number;

    /* Process an exponent string */
    if (*p == 'e' || *p == 'E') {
        /* Handle optional sign */
        negative = 0;
        switch (*++p) {
        case '-':
            negative = 1;
            p++;
            break;
        case '+':
            p++;
            break;
        }

        /* Process string of digits */
        n = 0;
        while (isdigit(*p)) {
            n = n * 10 + (*p - '0');
            p++;
        }

        if (negative)
            exponent -= n;
        else
            exponent += n;
    }

    if (exponent < -307 || exponent > 308) {
        return 0.0;
    }

    /* Scale the result */
    p10 = 10.;
    n = exponent;
    if (n < 0)
        n = -n;
    while (n) {
        if (n & 1) {
            if (exponent < 0)
                number /= p10;
            else
                number *= p10;
        }
        n >>= 1;
        p10 *= p10;
    }

    if (endptr)
        *endptr = p;

    return number;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    const unsigned char *p1 = (const unsigned char *)s1,
                        *p2 = (const unsigned char *)s2;
    while (n-- > 0) {
        if (*p1 != *p2)
            return *p1 - *p2;
        if (*p1 == '\0')
            return 0;
        p1++, p2++;
    }
    return 0;
}

char *ftoa(double f, char *buf, int precision) {
    char *ptr = buf;
    char *p = ptr;
    char *p1;
    char c;
    long intPart;

    if (precision > MAX_PRECISION)
        precision = MAX_PRECISION;
    if (f < 0) {
        f = -f;
        *ptr++ = '-';
    }
    if (precision < 0) {
        if (f < 1.0)
            precision = 6;
        else if (f < 10.0)
            precision = 5;
        else if (f < 100.0)
            precision = 4;
        else if (f < 1000.0)
            precision = 3;
        else if (f < 10000.0)
            precision = 2;
        else if (f < 100000.0)
            precision = 1;
        else
            precision = 0;
    }
    if (precision)
        f += rounders[precision];
    intPart = f;
    f -= intPart;
    if (!intPart)
        *ptr++ = '0';
    else {
        p = ptr;
        while (intPart) {
            *p++ = '0' + intPart % 10;
            intPart /= 10;
        }
        p1 = p;
        while (p > ptr) {
            c = *--p;
            *p = *ptr;
            *ptr++ = c;
        }
        ptr = p1;
    }
    if (precision) {
        *ptr++ = '.';
        while (precision--) {
            f *= 10.0;
            c = f;
            *ptr++ = '0' + c;
            f -= c;
        }
    }
    *ptr = 0;
    return ptr;
}