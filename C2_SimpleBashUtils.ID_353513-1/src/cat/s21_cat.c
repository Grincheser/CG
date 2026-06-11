#include "s21_cat.h"

CatFlags parse_flags(char *arg, CatFlags flags) {
    if (arg[0] == '-' && arg[1] == '-' && arg[2] != '\0') {
        if (strcmp(arg, "--number-nonblank") == 0) flags.b = 1;
        else if (strcmp(arg, "--number") == 0) flags.n = 1;
        else if (strcmp(arg, "--squeeze-blank") == 0) flags.s = 1;
        else flags.error = 1;
    } else if (arg[0] == '-' && arg[1] != '\0') {
        for (size_t i = 1; i < strlen(arg) && !flags.error; i++) {
            switch (arg[i]) {
                case 'b': flags.b = 1; break;
                case 'e': flags.e = 1; flags.v = 1; break;
                case 'E': flags.E = 1; break;
                case 'n': flags.n = 1; break;
                case 's': flags.s = 1; break;
                case 't': flags.t = 1; flags.v = 1; break;
                case 'T': flags.T = 1; break;
                case 'v': flags.v = 1; break;
                default: flags.error = 1; break;
            }
        }
    }
    return flags;
}

void print_non_printable(int c) {
    if (c == 127) {
        printf("^?");
    } else if (c > 127) {
        printf("M-");
        if (c >= 128 + 32) {
            printf("%c", c - 128);
        } else {
            printf("^%c", c - 128 + 64);
        }
    } else {
        printf("^%c", c + 64);
    }
}

void process_file(FILE *f, CatFlags flags) {
    int current_char;
    int prev_char = '\n';
    int squeeze_backlog = 0;
    int line_number = 1;

    while ((current_char = fgetc(f)) != EOF) {
        if (flags.s && prev_char == '\n' && current_char == '\n' && squeeze_backlog) {
        } else {
            if (prev_char == '\n' && current_char == '\n') {
                squeeze_backlog = 1;
            } else if (current_char != '\n') {
                squeeze_backlog = 0;
            }

            if (prev_char == '\n') {
                if (flags.b && current_char != '\n') {
                    printf("%6d\t", line_number++);
                } else if (!flags.b && flags.n) {
                    printf("%6d\t", line_number++);
                }
            }

            if ((flags.t || flags.T) && current_char == '\t') {
                printf("^I");
            } else {
                if ((flags.e || flags.E) && current_char == '\n') {
                    printf("$");
                }
                if (flags.v && current_char != '\n' && current_char != '\t' && 
                   (current_char < 32 || current_char >= 127)) {
                    print_non_printable(current_char);
                } else {
                    putchar(current_char);
                }
            }
            prev_char = current_char;
        }
    }
}

int main(int argc, char *argv[]) {
    CatFlags flags = {0}; 
    int has_error = 0;
    int file_processed = 0;

    for (int i = 1; i < argc && !has_error; i++) {
        if (argv[i][0] == '-') {
            flags = parse_flags(argv[i], flags);
            if (flags.error) {
                fprintf(stderr, "s21_cat: illegal option\n");
                has_error = 1;
            }
        }
    }

    if (!has_error) {
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] != '-') {
                FILE *f = fopen(argv[i], "r");
                if (f == NULL) {
                    fprintf(stderr, "s21_cat: %s: No such file or directory\n", argv[i]);
                } else {
                    process_file(f, flags);
                    fclose(f);
                }
                file_processed = 1;
            }
        }

        if (!file_processed) {
            process_file(stdin, flags);
        }
    }

    return has_error ? 1 : 0;
}