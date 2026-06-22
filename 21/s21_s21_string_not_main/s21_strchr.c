#include "s21_string.h"

char *s21_strchr(const char *str, int ch) {
    char *result = S21_NULL;

    while (*str != '\0' && *str != (char)ch) {
        str++;
    }

    if (*str == (char)ch) {
        result = (char *)str;
    }
    
    return result;
}