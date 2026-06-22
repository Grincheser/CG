#include "tests.h"

static void check_strlen(const char *s) {
    ck_assert_uint_eq(s21_strlen(s), strlen(s));
}

START_TEST(test_strlen_empty) {
    check_strlen("");
}
END_TEST

START_TEST(test_strlen_normal) {
    check_strlen("Hello world");
}
END_TEST

START_TEST(test_strlen_null_in_middle) {
    char arr[] = "ab\0cd";
    check_strlen(arr);
}
END_TEST

Suite *s21_strlen_suite(void) {
    Suite *s = suite_create("s21_strlen");
    TCase *tc = tcase_create("Basic");

    tcase_add_test(tc, test_strlen_empty);
    tcase_add_test(tc, test_strlen_normal);
    tcase_add_test(tc, test_strlen_null_in_middle);

    suite_add_tcase(s, tc);
    return s;
}