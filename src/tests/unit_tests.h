#ifndef UNIT_TESTS_H
#define UNIT_TESTS_H

#include <check.h>
#include "../s21_matrix.h"

enum ERROR_CODES { OK, INCORRECT_MATRIX, CALCULATION_ERROR };

// void s21_initialize_matrix(matrix_t *A, double start_value,
//                            double iteration_step);

Suite *s21_create_matrix_suite(void);

Suite *s21_eq_matrix_suite(void);

Suite *s21_remove_matrix_suite(void);

Suite *s21_sum_matrix_suite(void);

Suite *s21_sub_matrix_suite(void);

Suite *s21_mult_number_suite(void);

Suite *s21_mult_matrix_suite(void);

Suite *s21_transpose_suite(void);

Suite *s21_calc_complements_suite(void);

Suite *s21_determinant_suite(void);

Suite *s21_inverse_matrix_suite(void);

#endif  // UNIT_TESTS_H