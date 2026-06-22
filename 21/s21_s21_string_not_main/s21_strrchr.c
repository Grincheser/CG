#include "s21_string.h"

char *s21_strrchr(const char *str, int ch) {
    char *result = S21_NULL;

    while (*str != '\0') {
        if (*str == (char)ch) {
            result = (char *)str;
        }
        str++;
    }

    if ((char)ch == '\0') {
        result = (char *)str;
    }

    return result;
}