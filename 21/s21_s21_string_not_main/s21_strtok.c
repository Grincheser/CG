#include "s21_string.h"

char *s21_strtok(char *str, const char *delim) {

  int check2 = 0;
  int check1 = 0;
  static char *pa1;
  char *res = S21_NULL;

  if (str != S21_NULL) {
    for (int i = 0; check1 == 0; i++) {
      if (str[i] == *delim) {
        check1 = 1;
        str[i] = '\0';
        pa1 = &str[i + 1];
        res = str;
      } else if (str[i] == '\0') {
        check1 = 1;
        pa1 = S21_NULL;
        res = str;
      }
    }
  } else {
    if (pa1 != S21_NULL) {
      char *pa2 = pa1;

      while (check2 != 1) {
        if (*pa1 == '\0') {
          check2 = 1;
          pa1 = S21_NULL;
        } else if (*pa1 == *delim) {
          *pa1 = '\0';
          pa1++;
          check2 = 1;
        } else {
          pa1++;
        }
      }
      res = pa2;
    }
  }

  return res;
}