#include "s21_string.h"

char *s21_strpbrk(const char *str, const char *accept) {
    char *result = S21_NULL;
    while (*str != '\0' && result == S21_NULL) {
        const char *ptr_accept = accept;
        while (*ptr_accept != '\0' && result == S21_NULL) {
            if (*str == *ptr_accept) {
                result = (char *)str;
            }
            ptr_accept++;
        }
        str++;
    }
    return result;
}