#include "s21_string.h"

s21_size_t s21_strlen(const char *str_start) {
    const char *str_end = str_start;

    while (*str_end != '\0') {
        str_end++;
    }

    return (s21_size_t)(str_end - str_start);
}