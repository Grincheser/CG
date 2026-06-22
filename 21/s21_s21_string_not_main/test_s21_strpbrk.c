#include "tests.h"

static void check_strpbrk(const char *str, const char *accept) {
    ck_assert_ptr_eq(s21_strpbrk(str, accept), strpbrk(str, accept));
}

START_TEST(test_strpbrk_basic) {
    check_strpbrk("Hello!", "lo");
}
END_TEST

START_TEST(test_strpbrk_first_symbol) {
    check_strpbrk("Hello!", "H");
}
END_TEST

START_TEST(test_strpbrk_case_sensitive) {
    check_strpbrk("Hello!", "h");
}
END_TEST

START_TEST(test_strpbrk_last_symbol) {
    check_strpbrk("Hello!", "!");
}
END_TEST

START_TEST(test_strpbrk_several_coincidences) {
    check_strpbrk("Hello!", "ole");
}
END_TEST

START_TEST(test_strpbrk_repeat_symbols) {
    check_strpbrk("asbbpbrtbjlkl", "bbbb");
}
END_TEST

START_TEST(test_strpbrk_no_match) {
    check_strpbrk("aspbrtjlkl", "xmh");
}
END_TEST

START_TEST(test_strpbrk_null_str) {
    check_strpbrk("", "xmh");
}
END_TEST

START_TEST(test_strpbrk_null_accept) {
    check_strpbrk("xmh", "");
}
END_TEST

START_TEST(test_strpbrk_null_str_null_accept) {
    check_strpbrk("", "");
}
END_TEST


Suite *s21_strpbrk_suite(void) {
    Suite *s = suite_create("s21_strpbrk");
    TCase *tc = tcase_create("Basic");

    tcase_add_test(tc, test_strpbrk_basic);
    tcase_add_test(tc, test_strpbrk_first_symbol);
    tcase_add_test(tc, test_strpbrk_case_sensitive);
    tcase_add_test(tc, test_strpbrk_last_symbol);
    tcase_add_test(tc, test_strpbrk_several_coincidences);
    tcase_add_test(tc, test_strpbrk_repeat_symbols);
    tcase_add_test(tc, test_strpbrk_no_match);
    tcase_add_test(tc, test_strpbrk_null_str);
    tcase_add_test(tc, test_strpbrk_null_accept);
    tcase_add_test(tc, test_strpbrk_null_str_null_accept);

    suite_add_tcase(s, tc);
    return s;
}