#include "s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n){
    const unsigned char *str_h1 = (unsigned char *)str1;
    const unsigned char *str_h2 = (unsigned char *)str2;
    int alpha = 0;

    for(s21_size_t i = 0; i < n && alpha == 0; i ++){
        alpha = str_h1[i] - str_h2[i]; 
    } 
    return alpha;

}