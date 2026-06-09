#ifndef S21_GREP_H
#define S21_GREP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h> // Линуксовая библиотека регулярных выражений

// Структура для хранения флагов и паттерна
typedef struct {
    int i; // Игнорировать регистр (REG_ICASE)
    int v; // Инвертировать поиск (выводить несовпавшие строки)
    int c; // Выводить только количество совпавших строк
    int l; // Выводить только имена совпавших файлов
    int n; // Печатать номер строки перед совпадением
    int h; // Скрывать имена файлов при выводе
    int s; // Подавлять сообщения об ошибках (если файл не найден)
    char pattern[4096]; // Буфер, куда мы сохраняем искомое слово
    int is_valid;       // 1 - если всё распарсилось ок, 0 - если была ошибка
} GrepFlags;

// Объявление функций
GrepFlags parse_flags(int argc, char *argv[], GrepFlags flags);
void process_file(char *filename, GrepFlags flags, int multiple_files);

#endif