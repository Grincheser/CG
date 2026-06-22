#include "tests.h"

static void check_memset(int c, s21_size_t n) {
    char buf_std[100] = {0};
    char buf_s21[100] = {0};

    void *ptr_std = memset(buf_std, c, n);
    void *ptr_s21 = s21_memset(buf_s21, c, n);

    ck_assert_mem_eq(ptr_std, ptr_s21, n);
    ck_assert_ptr_eq(ptr_std, buf_std);
    ck_assert_ptr_eq(ptr_s21, buf_s21);
}

START_TEST(test_memset_basic) {
    check_memset('X', 10);
}
END_TEST

START_TEST(test_memset_zero_fill) {
    check_memset(0, 50);
}
END_TEST

START_TEST(test_memset_n_zero) {
    check_memset('A', 0);
}
END_TEST

Suite *s21_memset_suite(void) {
    Suite *s = suite_create("s21_memset");
    TCase *tc = tcase_create("Basic");

    tcase_add_test(tc, test_memset_basic);
    tcase_add_test(tc, test_memset_zero_fill);
    tcase_add_test(tc, test_memset_n_zero);

    suite_add_tcase(s, tc);
    return s;
}