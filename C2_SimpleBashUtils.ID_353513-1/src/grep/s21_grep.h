#ifndef S21_GREP_H
#define S21_GREP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

typedef struct {
    int i;              // Игнорировать регистр
    int v;              // Инвертировать поиск
    int c;              // Только количество строк
    int l;              // Только имена файлов
    int n;              // Номера строк
    int h;              // Скрывать имена файлов
    int s;              // Подавлять ошибки
    int e;              // Флаг паттерна
    char pattern[4096]; // Буфер паттернов
    int is_valid;       // Валидность флагов
} GrepFlags;

GrepFlags parse_flags(int argc, char *argv[]);
void print_line(char *line, GrepFlags flags, char *filename, int line_num, int multi);
void process_stream(FILE *f, GrepFlags flags, char *filename, int multiple_files);
void open_and_process(char *filename, GrepFlags flags, int multi);
void handle_files(int argc, char *argv[], GrepFlags flags);

#endif