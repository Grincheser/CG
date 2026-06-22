#include "tests.h"

static void check_strcspn(const char *str1, const char *str2) {
    ck_assert_uint_eq(s21_strcspn(str1, str2), strcspn(str1, str2));
}

START_TEST(test_strcspn_empty) {
    check_strcspn("", "");
}
END_TEST

START_TEST(test_strcspn_normal) {
    check_strcspn("aaaaabbbcc", "c338ghjtgkltrtl;.,");
}
END_TEST

START_TEST(test_strcspn_empty_alf) {
    check_strcspn("abdvfvdhfodfvdf", "");
}
END_TEST

START_TEST(test_strcspn_special_symbols) {
    check_strcspn("!@@#$&*()+_-~`", "&*)~");
}
END_TEST


Suite *s21_strcspn_suite(void) {
    Suite *s = suite_create("s21_strcspn");
    TCase *tc = tcase_create("Basic");

    tcase_add_test(tc, test_strcspn_empty);
    tcase_add_test(tc, test_strcspn_normal);
    tcase_add_test(tc, test_strcspn_empty_alf);
    tcase_add_test(tc, test_strcspn_special_symbols);


    suite_add_tcase(s, tc);
    return s;
}