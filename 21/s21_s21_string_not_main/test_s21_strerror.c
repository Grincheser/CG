#include "tests.h"

static void check_strerror(int errnum) {
    char *alpha_array = s21_strerror(errnum);
    char *beta_array = strerror(errnum);
    ck_assert_str_eq(alpha_array, beta_array);
}

START_TEST(test_strerror_range) {
    for (int i = -100; i < 355; i ++){
        check_strerror(i);
    }
}
END_TEST

Suite *s21_strerror_suite(void) {
    Suite *s = suite_create("s21_strerror");
    TCase *tc = tcase_create("Basic");

    tcase_add_test(tc, test_strerror_range);

    suite_add_tcase(s, tc);
    return s;
}