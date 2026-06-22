#ifndef S21_STRING_H_
#define S21_STRING_H_
#include <stdarg.h>  

typedef unsigned long s21_size_t;

#define S21_NULL ((void *)0)

s21_size_t s21_strlen(const char *str_start);
void *s21_memset(void *str, int ch, s21_size_t n);
void *s21_memchr(const void *str, int c, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
char *s21_strchr(const char *str, int ch);
char *s21_strrchr(const char *str, int ch);
char *s21_strstr(const char *haystack, const char *needle);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_int_to_char(int errnum); // int -> *char array вернет
char* s21_strerror(int errnum);
char *s21_strpbrk(const char *str, const char *accept);
s21_size_t s21_strcspn(const char *str1, const char *str2);

int s21_sprintf(char *str, const char *format, ...);
#endif

