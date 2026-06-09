#ifndef S21_CAT_H
#define S21_CAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int b;     // --number-nonblank
    int e;     // -e (выводит $ + v)
    int E;     // -E (только $)
    int n;     // --number
    int s;     // --squeeze-blank
    int t; // -t (выводит ^I + v)
    int T; // -T (только ^I)
    int v;     // управление отображением непечатаемых символов
    int error; // <-- Сюда пишем 1, если поймали неизвестный флаг
} CatFlags;

// Обновленная сигнатура функции — теперь возвращает структуру
CatFlags parse_flags(char *arg, CatFlags flags);
void process_file(FILE *f, CatFlags flags);

#endif // S21_CAT_H