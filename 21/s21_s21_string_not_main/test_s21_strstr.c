#include "tests.h"

static void check_strstr(const char *haystack, const char *needle) {
    ck_assert_ptr_eq(s21_strstr(haystack, needle), strstr(haystack, needle));
}

START_TEST(test_strstr_basic) {
    check_strstr("Hello, world!", "llo, w");
}
END_TEST

START_TEST(test_strstr_not_found ) {
    check_strstr("Hello", "World");
}
END_TEST

START_TEST(test_strstr_empty_haystack) {
    check_strstr("", "abc");
}
END_TEST

START_TEST(test_strstr_empty_needle) {
    check_strstr("abc", "");
}
END_TEST

START_TEST(test_strstr_partial_match_before_real) {
    check_strstr("aaab", "aab");
}
END_TEST


Suite *s21_strstr_suite(void) {
    Suite *s = suite_create("s21_strstr");
    TCase *tc = tcase_create("Basic");

    tcase_add_test(tc, test_strstr_basic);
    tcase_add_test(tc, test_strstr_not_found);
    tcase_add_test(tc, test_strstr_empty_haystack);
    tcase_add_test(tc, test_strstr_empty_needle);
    tcase_add_test(tc, test_strstr_partial_match_before_real);

    suite_add_tcase(s, tc);
    return s;
}