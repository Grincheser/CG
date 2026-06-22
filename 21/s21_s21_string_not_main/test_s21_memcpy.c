#include "tests.h"

static void check_memcpy(const char *src, s21_size_t n) {
    char buf_dest_std[100] = {0};
    char buf_dest_s21[100] = {0};

    void *ptr_std = memcpy(buf_dest_std, src, n);
    void *ptr_s21 = s21_memcpy(buf_dest_s21, src, n);

    ck_assert_mem_eq(ptr_std, ptr_s21, n);
    ck_assert_ptr_eq(ptr_std, buf_dest_std);
    ck_assert_ptr_eq(ptr_s21, buf_dest_s21);
}

START_TEST(test_memcpy_zero_n) {
    check_memcpy("Hello", 0);
}
END_TEST

START_TEST(test_memcpy_basic) {
    check_memcpy("Hello", 5);
}
END_TEST

START_TEST(test_memcpy_null_in_middle) {
    check_memcpy("ab\0cd", 5);
}
END_TEST

Suite *s21_memcpy_suite(void) {
    Suite *s = suite_create("s21_memcpy");
    TCase *tc = tcase_create("Basic");

    tcase_add_test(tc, test_memcpy_zero_n);
    tcase_add_test(tc, test_memcpy_basic);
    tcase_add_test(tc, test_memcpy_null_in_middle);

    suite_add_tcase(s, tc);
    return s;
}