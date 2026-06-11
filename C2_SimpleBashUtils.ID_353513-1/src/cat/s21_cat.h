#ifndef S21_CAT_H
#define S21_CAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int b;      // --number-nonblank
    int e;      // -e
    int E;      // -E
    int n;      // --number
    int s;      // --squeeze-blank
    int t;      // -t
    int T;      // -T
    int v;      // -v
    int error; 
} CatFlags;

CatFlags parse_flags(char *arg, CatFlags flags);
void print_non_printable(int c);
void process_file(FILE *f, CatFlags flags);

#endif