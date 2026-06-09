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

void process_file(FILE *f, CatFlags flags) {
    int current_char;
    int prev_char = '\n';
    int squeeze_backlog = 0;
    int line_number = 1;

    while ((current_char = fgetc(f)) != EOF) {
        // s
        if (prev_char == '\n' && current_char == '\n') {
            if (flags.s && squeeze_backlog) {
                continue; 
            }
            squeeze_backlog = 1;
        } else if (current_char != '\n') {
            squeeze_backlog = 0; 
        }

        // b n
        if (prev_char == '\n') {
            if (flags.b) {
                if (current_char != '\n') {
                    printf("%6d\t", line_number++);
                }
            } else if (flags.n) {
                printf("%6d\t", line_number++);
            }
        }

        // t T
        if ((flags.t || flags.T) && current_char == '\t') {
            printf("^I");
            prev_char = current_char;
            continue;
        }

        // e E
        if ((flags.e || flags.E) && current_char == '\n') {
            printf("$");
        }

        // v 
        if (flags.v && current_char != '\n' && current_char != '\t') {
            if (current_char >= 127) {
                if (current_char == 127) {
                    printf("^?");
                } else {
                    printf("M-");
                    if (current_char >= 128 + 32) printf("%c", current_char - 128);
                    else printf("^%c", current_char - 128 + 64);
                }
                prev_char = current_char;
                continue;
            } else if (current_char < 32) {
                printf("^%c", current_char + 64);
                prev_char = current_char;
                continue;
            }
        }

        putchar(current_char);
        prev_char = current_char;
    }
}

int main(int argc, char *argv[]) {
    CatFlags flags = {0}; 
    int has_error = 0;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            flags = parse_flags(argv[i], flags);
            if (flags.error) {
                fprintf(stderr, "s21_cat: illegal option\n");
                has_error = 1;
                break;
            }
        }
    }

    if (has_error) return 1;

    int file_processed = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            FILE *f = fopen(argv[i], "r");
            if (f == NULL) {
                fprintf(stderr, "s21_cat: %s: No such file or directory\n", argv[i]);
                continue;
            }
            process_file(f, flags);
            fclose(f);
            file_processed = 1;
        }
    }

    return 0;
}