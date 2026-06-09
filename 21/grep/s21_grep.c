#include "s21_grep.h"

// Функция для разбора флагов и поиска паттерна
GrepFlags parse_flags(int argc, char *argv[], GrepFlags flags) {
    int pattern_found = 0;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            // Обработка коротких флагов, в том числе склеенных (например, -iv)
            for (int j = 1; argv[i][j] != '\0'; j++) {
                switch (argv[i][j]) {
                    case 'i': flags.i = 1; break;
                    case 'v': flags.v = 1; break;
                    case 'c': flags.c = 1; break;
                    case 'l': flags.l = 1; break;
                    case 'n': flags.n = 1; break;
                    case 'h': flags.h = 1; break;
                    case 's': flags.s = 1; break;
                    default: 
                        // Если флаг неверный и нет флага тишины -s, выводим ошибку
                        if (!flags.s) {
                            fprintf(stderr, "s21_grep: illegal option -- %c\n", argv[i][j]);
                        }
                        flags.is_valid = 0;
                        return flags;
                }
            }
        } else {
            // Если аргумент без минуса — это паттерн (берём только самый первый)
            if (pattern_found == 0) {
                strcpy(flags.pattern, argv[i]);
                pattern_found = 1; 
            }
        }
    }

    flags.is_valid = pattern_found;
    return flags;
}

// Функция обработки одного конкретного файла
void process_file(char *filename, GrepFlags flags, int multiple_files) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        // Ошибку печатаем только если НЕ включен флаг тишины -s
        if (!flags.s) {
            fprintf(stderr, "s21_grep: %s: No such file or directory\n", filename);
        }
        return;
    }

    // Переменные для построчного чтения через getline
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
    int line_number = 0; // Счетчик текущей строки
    int match_count = 0; // Счетчик совпадений в файле

    // Настройка регулярного выражения
    regex_t regex;
    int cflags = REG_EXTENDED;
    if (flags.i) {
        cflags |= REG_ICASE; // Добавляем флаг игнорирования регистра
    }

    // Компилируем паттерн в понятный для библиотеки regex вид
    if (regcomp(&regex, flags.pattern, cflags) != 0) {
        fclose(f);
        return;
    }

    // Читаем файл построчно до самого конца
    while ((read = getline(&line, &len, f)) != -1) {
        line_number++;
        
        // Функция regexec ищет совпадение регулярки в текущей строке line
        int status = regexec(&regex, line, 0, NULL, 0);

        // Логика совпадения с учетом флага инверсии -v
        int is_match = (status == 0 && !flags.v) || (status == REG_NOMATCH && flags.v);

        if (is_match) {
            match_count++;
            
            // Если нужен только список файлов с совпадениями (-l), останавливаем чтение
            if (flags.l) {
                break;
            }

            // Если НЕ стоит флаг подсчета количества (-c), выводим строку на экран
            if (!flags.c) {
                // Печатаем имя файла, если файлов много и не активен флаг -h
                if (multiple_files && !flags.h) {
                    printf("%s:", filename);
                }
                // Печатаем номер строки, если активен флаг -n
                if (flags.n) {
                    printf("%d:", line_number);
                }
                // Печатаем саму строку из файла
                printf("%s", line);
                
                // На случай, если строка в конце файла не имела символа переноса \n
                if (read > 0 && line[read - 1] != '\n') {
                    printf("\n");
                }
            }
        }
    }

    // Пост-обработка после закрытия цикла чтения файла

    // Вывод для флага -c (Количество совпадений)
    if (flags.c) {
        if (multiple_files && !flags.h) {
            printf("%s:", filename);
        }
        // По стандарту grep: если совмещены -c и -l, пишется 1 при наличии совпадений, иначе 0
        if (flags.l && match_count > 0) {
            match_count = 1;
        }
        printf("%d\n", match_count);
    }

    // Вывод для флага -l (Имя файла при наличии совпадений)
    if (flags.l && match_count > 0) {
        printf("%s\n", filename);
    }

    // Чистим выделенную getline память, закрываем регулярку и файл
    free(line);
    regfree(&regex);
    fclose(f);
}

int main(int argc, char *argv[]) {
    // Базовая проверка на запуск без аргументов
    if (argc < 2) {
        return 1;
    }

    GrepFlags flags = {0}; // Полностью зануляем структуру флагов

    // Передаем структуру по значению и ловим измененный результат обратно
    flags = parse_flags(argc, argv, flags);

    // Если парсер вернул 0 (не найден паттерн или некорректный флаг) — выходим
    if (flags.is_valid == 0) {
        return 1; 
    }

    int file_count = 0;
    int pattern_skipped = 0;

    // Шаг 1. Считаем, сколько файлов нам передали в аргументах
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (pattern_skipped == 0) {
                pattern_skipped = 1; // Пропускаем первый не-флаг, так как это паттерн
            } else {
                file_count++; // Все последующие не-флаги — это файлы
            }
        }
    }

    pattern_skipped = 0;
    // Флаг множества файлов активен, если файлов больше чем 1
    int multiple_files = (file_count > 1);

    // Шаг 2. Запускаем обработку для каждого файла
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (pattern_skipped == 0) {
                pattern_skipped = 1; // Снова пропускаем аргумент-паттерн
            } else {
                // Отправляем файл на обработку
                process_file(argv[i], flags, multiple_files);
            }
        }
    }

    return 0;
}