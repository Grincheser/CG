#include "tests.h"

static int sign(int x) { return (x > 0) - (x < 0); }

//s21_strncmp
static void check_strncmp(const void *str1, const void *str2, s21_size_t n){
    ck_assert_int_eq(sign(s21_strncmp(str1, str2, n)), sign(strncmp(str1, str2, n)));
}

START_TEST(test_strncmp_equality){
    check_strncmp("abc", "abc", 3);
}
END_TEST

START_TEST(test_strncmp_first_number_less){
    check_strncmp("abc", "bbc", 3);
}
END_TEST

START_TEST(test_strncmp_first_number_more){
    check_strncmp("bbc", "abc", 3);
}
END_TEST

START_TEST(test_strncmp_random_last_bite){
    check_strncmp("abc", "abd", 3);
}
END_TEST

START_TEST(test_strncmp_n_is_zero){
    check_strncmp("abc", "xyz", 0);
}
END_TEST

START_TEST(test_strncmp_str_with_0){
    check_strncmp("a\0c", "a\0c", 3);
}
END_TEST

Suite *s21_strncmp_suite(void){
    Suite *s = suite_create("s21_strncmp");
    TCase *tc = tcase_create("Basic");

    tcase_add_test(tc,test_strncmp_equality);
    tcase_add_test(tc,test_strncmp_first_number_less);
    tcase_add_test(tc,test_strncmp_first_number_more);
    tcase_add_test(tc,test_strncmp_random_last_bite);
    tcase_add_test(tc,test_strncmp_n_is_zero);
    tcase_add_test(tc,test_strncmp_str_with_0);

    suite_add_tcase(s, tc);
    return s;
}