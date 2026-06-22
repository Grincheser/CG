#include "tests.h"

static void check_strrchr(const char *src, int ch) {
    ck_assert_ptr_eq(s21_strrchr(src, ch), strrchr(src, ch));
}

START_TEST(test_strrchr_basic) {
    check_strrchr("Hello", 'l');
}
END_TEST

START_TEST(test_strrchr_null_symbol) {
    check_strrchr("Hello", '\0');
}
END_TEST

START_TEST(test_strrchr_not_exist) {
    check_strrchr("Hello", 'X');
}
END_TEST

Suite *s21_strrchr_suite(void) {
    Suite *s = suite_create("s21_strrchr");
    TCase *tc = tcase_create("Basic");

    tcase_add_test(tc, test_strrchr_basic);
    tcase_add_test(tc, test_strrchr_null_symbol);
    tcase_add_test(tc, test_strrchr_not_exist);

    suite_add_tcase(s, tc);
    return s;
}