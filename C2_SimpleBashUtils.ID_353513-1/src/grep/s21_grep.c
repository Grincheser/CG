#include "s21_grep.h"

GrepFlags parse_flags(int argc, char *argv[]) {
    GrepFlags flags = {0};
    flags.is_valid = 1;
    int pattern_found = 0;

    for (int i = 1; i < argc && flags.is_valid; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j] != '\0' && flags.is_valid; j++) {
                if (argv[i][j] == 'e') {
                    flags.e = 1;
                    if (i + 1 < argc) {
                        if (pattern_found) strcat(flags.pattern, "|");
                        strcat(flags.pattern, argv[i + 1]);
                        pattern_found = 1;
                        i++;
                    } else { flags.is_valid = 0; }
                    break;
                } else if (argv[i][j] == 'i') flags.i = 1;
                else if (argv[i][j] == 'v') flags.v = 1;
                else if (argv[i][j] == 'c') flags.c = 1;
                else if (argv[i][j] == 'l') flags.l = 1;
                else if (argv[i][j] == 'n') flags.n = 1;
                else if (argv[i][j] == 'h') flags.h = 1;
                else if (argv[i][j] == 's') flags.s = 1;
                else {
                    if (!flags.s) fprintf(stderr, "s21_grep: illegal option -- %c\n", argv[i][j]);
                    flags.is_valid = 0;
                }
            }
        } else if (!flags.e && !pattern_found) {
            strcpy(flags.pattern, argv[i]);
            pattern_found = 1;
        }
    }
    if (!pattern_found) flags.is_valid = 0;
    return flags; // Единственный выход из функции
}

void print_line(char *line, GrepFlags flags, char *filename, int line_num, int multi) {
    if (!flags.c && !flags.l) {
        if (multi && !flags.h) {
            printf("%s:", filename);
        }
        if (flags.n) {
            printf("%d:", line_num);
        }
        printf("%s", line);
    }
}

void process_stream(FILE *f, GrepFlags flags, char *filename, int multiple_files) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int line_number = 0, match_count = 0;
    regex_t regex;
    int cflags = REG_EXTENDED | (flags.i ? REG_ICASE : 0);

    if (regcomp(&regex, flags.pattern, cflags) == 0) {

        while ((read = getline(&line, &len, f)) != -1 && !(flags.l && match_count > 0)) {
            line_number++;
            int status = regexec(&regex, line, 0, NULL, 0);
            int is_match = (status == 0 && !flags.v) || (status == REG_NOMATCH && flags.v);
            if (is_match) {
                match_count++;
                print_line(line, flags, filename, line_number, multiple_files);
                if (read > 0 && line[read - 1] != '\n' && !flags.c && !flags.l) {
                    printf("\n");
                }
            }
        }
        if (flags.c) {
            if (multiple_files && !flags.h) printf("%s:", filename);
            printf("%d\n", flags.l && match_count > 0 ? 1 : match_count);
        }
        if (flags.l && match_count > 0) printf("%s\n", filename);
        free(line);
        regfree(&regex);
    }
}

void open_and_process(char *filename, GrepFlags flags, int multi) {
    FILE *f = fopen(filename, "r");
    if (f == NULL && !flags.s) {
        fprintf(stderr, "s21_grep: %s: No such file or directory\n", filename);
    } else if (f != NULL) {
        process_stream(f, flags, filename, multi);
        fclose(f);
    }
}

void handle_files(int argc, char *argv[], GrepFlags flags) {
    int file_count = 0;
    int pattern_skipped = flags.e;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j] != '\0'; j++) {
                if (argv[i][j] == 'e') i++;
            }
        } else {
            if (!pattern_skipped) pattern_skipped = 1;
            else file_count++;
        }
    }

    int multiple_files = (file_count > 1);
    pattern_skipped = flags.e;

    if (file_count == 0) {
        process_stream(stdin, flags, "(standard input)", multiple_files);
    } else {
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] == '-') {
                for (int j = 1; argv[i][j] != '\0'; j++) {
                    if (argv[i][j] == 'e') i++;
                }
            } else {
                if (!pattern_skipped) pattern_skipped = 1;
                else open_and_process(argv[i], flags, multiple_files);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }
    GrepFlags flags = parse_flags(argc, argv);
    if (flags.is_valid) {
        handle_files(argc, argv, flags);
    }
    return flags.is_valid ? 0 : 1;
}