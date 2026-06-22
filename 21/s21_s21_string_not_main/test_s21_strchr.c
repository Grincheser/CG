#include "tests.h"

static void check_strchr(const char *src, int ch) {
    ck_assert_ptr_eq(s21_strchr(src, ch), strchr(src, ch));
}

START_TEST(test_strchr_basic) {
    check_strchr("Hello", 'l');
}
END_TEST

START_TEST(test_strchr_null_symbol) {
    check_strchr("Hello", '\0');
}
END_TEST

START_TEST(test_strchr_not_exist) {
    check_strchr("Hello", 'X');
}
END_TEST

Suite *s21_strchr_suite(void) {
    Suite *s = suite_create("s21_strchr");
    TCase *tc = tcase_create("Basic");

    tcase_add_test(tc, test_strchr_basic);
    tcase_add_test(tc, test_strchr_null_symbol);
    tcase_add_test(tc, test_strchr_not_exist);

    suite_add_tcase(s, tc);
    return s;
}