#include "s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
    unsigned char table[256] = {0};

    while (*str2) {
        table[(unsigned char)*str2] = 1;
        str2++;
    }
    s21_size_t count = 0;
    while (str1[count]) {
        if (table[(unsigned char)str1[count]] != 0) {
            break;
        }
        count++;
    }
    return count;
}

