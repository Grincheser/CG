#include "tests.h"

static void check_strncat(char *dest, const char *src, s21_size_t n){
    char s21_test_dest_s21_strncat[50];
    char s21_test_dest_strncat[50];
    strcpy(s21_test_dest_s21_strncat, dest); 
    strcpy(s21_test_dest_strncat, dest);

    ck_assert_pstr_eq(s21_strncat(s21_test_dest_s21_strncat, src, n), strncat(s21_test_dest_strncat, src, n));
}

START_TEST(test_strncat_normal){
    check_strncat("Hello, ", "world", 5);
}
END_TEST



START_TEST(test_strncat_reserve){
    check_strncat("Hello, ", "world", 10);
}
END_TEST

START_TEST(test_strncat_less){
    check_strncat("Hello, ", "world", 4);
}
END_TEST

START_TEST(test_strncat_empty_n){
    check_strncat("Hello, ", "world", 0);
}
END_TEST

START_TEST(test_strncat_empty_src){
    check_strncat("Hello, ", "", 5);
}
END_TEST

START_TEST(test_strncat_empty_dest){
    check_strncat("", "world", 5);
}
END_TEST

Suite *s21_strncat_suite(void) {
    Suite *s = suite_create("s21_strncat");
    TCase *tc = tcase_create("Basic");

    tcase_add_test(tc, test_strncat_normal);
    tcase_add_test(tc, test_strncat_reserve);
    tcase_add_test(tc, test_strncat_less);
    tcase_add_test(tc, test_strncat_empty_n);
    tcase_add_test(tc, test_strncat_empty_src);
    tcase_add_test(tc, test_strncat_empty_dest);

    suite_add_tcase(s, tc);
    return s;
}