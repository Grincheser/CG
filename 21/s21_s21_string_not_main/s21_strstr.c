#include "s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
    char *result = S21_NULL;

    if (*needle == '\0') {
        result = (char *)haystack;
    } else {
        while (*haystack != '\0' && result == S21_NULL) {
            const char *h_temp = haystack;
            const char *n_temp = needle;
            while (*n_temp != '\0' && *n_temp == *h_temp) {
                n_temp++;
                h_temp++;
            }
            if (*n_temp == '\0') {
                result = (char *)haystack;
            }
            haystack++;
        }
    }
    return result;
}