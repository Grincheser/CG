#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n){
    unsigned char *str_h = (unsigned char *)str;
    for (s21_size_t  i = 0; i < n; i ++){
        if (*str_h == (unsigned char)c){
            return str_h;
        }
        str_h++;
    }
    return S21_NULL;
}