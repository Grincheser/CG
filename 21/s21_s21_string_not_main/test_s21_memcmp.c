#include "tests.h"

static int sign(int x) { return (x > 0) - (x < 0); }


static void check_memcmp(const void *str1, const void *str2, s21_size_t n){
    ck_assert_int_eq(sign(s21_memcmp(str1, str2, n)), sign(memcmp(str1, str2, n)));
}
//int s21_memcmp(const void *str1, const void *str2, s21_size_t n)
//START_TEST(test_memchr_found) {
//   check_memchr("Hello world", 'o', 11);
//}
//END_TEST

START_TEST(test_memcmp_equality){
    check_memcmp("abc", "abc", 3);
}
END_TEST

START_TEST(test_memcmp_first_number_less){
    check_memcmp("abc", "bbc", 3);
}
END_TEST

START_TEST(test_memcmp_first_number_more){
    check_memcmp("bbc", "abc", 3);
}
END_TEST

START_TEST(test_memcmp_random_last_bite){
    check_memcmp("abc", "abd", 3);
}
END_TEST

START_TEST(test_memcmp_n_is_zero){
    check_memcmp("abc", "xyz", 0);
}
END_TEST

Suite *s21_memcmp_suite(void){
    Suite *s = suite_create("s21_memcmp");
    TCase *tc = tcase_create("Basic");

    tcase_add_test(tc,test_memcmp_equality);
    tcase_add_test(tc,test_memcmp_first_number_less);
    tcase_add_test(tc,test_memcmp_first_number_more);
    tcase_add_test(tc,test_memcmp_random_last_bite);
    tcase_add_test(tc,test_memcmp_n_is_zero);

    suite_add_tcase(s, tc);
    return s;
}