#include "tests.h"

static void check_strncpy(char *dest, const char *src, s21_size_t n){
    char s21_test_src[50];
    char s21_test_src_orig[50];
    memcpy(s21_test_src, dest, 50);
    memcpy(s21_test_src_orig, dest, 50);
    s21_strncpy(s21_test_src,src,n);
    strncpy(s21_test_src_orig,src,n);
    ck_assert_mem_eq(s21_test_src, s21_test_src_orig, n);
}

START_TEST(test_strncpy_normal){
    check_strncpy("", "world\0", 6);
}
END_TEST

START_TEST(test_strncpy_n_less){
    check_strncpy("", "world\0", 4);
}
END_TEST

START_TEST(test_strncpy_src_less){
    check_strncpy("", "wo", 6);
}
END_TEST

START_TEST(test_strncpy_src_empty){
    check_strncpy("", "", 6);
}
END_TEST

START_TEST(test_strncpy_n_empty){
    check_strncpy("", "world\0", 0);
}
END_TEST

START_TEST(test_strncpy_src_center_next_line){
    check_strncpy("", "wor\0ld", 6);
}
END_TEST

START_TEST(test_strncpy_dest_with_data){
    check_strncpy("hello", "world\0", 6);
}
END_TEST

Suite *s21_strncpy_suite(void) {
    Suite *s = suite_create("s21_strncpy");
    TCase *tc = tcase_create("Basic");

    tcase_add_test(tc, test_strncpy_normal);
    tcase_add_test(tc, test_strncpy_n_less);
    tcase_add_test(tc, test_strncpy_src_less);
    tcase_add_test(tc, test_strncpy_src_empty);
    tcase_add_test(tc, test_strncpy_n_empty);
    tcase_add_test(tc, test_strncpy_src_center_next_line);
    tcase_add_test(tc, test_strncpy_dest_with_data);

    suite_add_tcase(s, tc);
    return s;
}