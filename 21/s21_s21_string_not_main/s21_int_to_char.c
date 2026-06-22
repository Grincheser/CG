#include "s21_string.h"

char *s21_int_to_char(int errnum){//for (int i = 0; i < 137; i++) {
  //  printf("%s %d\n", strerror(i), i);
  //}
  char alpha[2000] = {0}; 
  int is_minus = 1;

  if (errnum < 0){
    is_minus = 0;
    errnum = -errnum;
  }

  int i = 0;
  while (errnum > 0){
    int b = errnum % 10;
    alpha[i] = b + '0';
    i++;
    errnum =  errnum / 10;
  }
  if(is_minus == 0){
    alpha[i] = '-';
    alpha[i+1] = '\0';
    i++;
  }else{
    alpha[i] = '\0';
  }
   
  int j = 0;
  static char new_alpha[2000] = {0};
  

  while(i > 0){
    i--;
    new_alpha[j] = alpha[i];
    j++; 
  }
  
  new_alpha[j] = '\0';

  return new_alpha;
  
}

