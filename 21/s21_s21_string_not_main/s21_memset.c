#include "s21_string.h"

void *s21_memset(void *str, int ch, s21_size_t n) {
    unsigned char *ptr = (unsigned char *)str;

    for (s21_size_t i = 0; i < n; i++) {
        ptr[i] = (unsigned char)ch;
    }

    return str;
}