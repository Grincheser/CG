#include "tests.h"

static void check_memchr(const char *str, int c, s21_size_t n) {
    ck_assert_ptr_eq(s21_memchr(str, c, n), memchr(str, c, n));
}

START_TEST(test_memchr_found) {
    check_memchr("Hello world", 'o', 11);
}
END_TEST

START_TEST(test_memchr_not_found) {
    check_memchr("Hello world", 'z', 11);
}
END_TEST

START_TEST(test_memchr_null_byte) {
    check_memchr("abc\0def", '\0', 7);
}
END_TEST

START_TEST(test_memchr_first_byte) {
    check_memchr("abcdef", 'a', 6);
}
END_TEST

Suite *s21_memchr_suite(void) {
    Suite *s = suite_create("s21_memchr");
    TCase *tc = tcase_create("Basic");

    tcase_add_test(tc, test_memchr_found);
    tcase_add_test(tc, test_memchr_not_found);
    tcase_add_test(tc, test_memchr_null_byte);
    tcase_add_test(tc, test_memchr_first_byte);

    suite_add_tcase(s, tc);
    return s;
}