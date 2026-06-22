#include "tests.h"

int main(void) {
    Suite *suites[] = {
        s21_strlen_suite(),
        s21_memset_suite(),
        s21_memchr_suite(),
        s21_memcmp_suite(),
        s21_memcpy_suite(),
        s21_strncat_suite(),
        s21_strncpy_suite(),
        s21_strchr_suite(),
        s21_strrchr_suite(),
        s21_strstr_suite(),
        s21_strncmp_suite(),
        s21_strerror_suite(),
        s21_strpbrk_suite(),
        s21_strcspn_suite(),
        NULL
    };
    
    int total_failed = 0;

    for (int i = 0; suites[i] != NULL; i++) {
        SRunner *runner = srunner_create(suites[i]);
        srunner_run_all(runner, CK_NORMAL);
        total_failed += srunner_ntests_failed(runner);
        srunner_free(runner);
    }

    return (total_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}