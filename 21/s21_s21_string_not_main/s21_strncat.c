#include "s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n){
    int a = 0;
    char* alpha = dest;

    while (a == 0)
    {
        if (*dest == '\0'){
            a = 1;
        }else{
            dest++;
        }
        
    }
    for (s21_size_t i = 0; i < n && *src != '\0'; i ++){
        *dest = *src;
        dest ++;
        src++;
    }
    *dest = '\0';
    return alpha;

    
}