#include "s21_matrix.h"

double **createMatrix(int rows, int columns) {
  double **matrix =
      malloc(rows * columns * sizeof(double) + rows * sizeof(double *));
  if (matrix != NULL) {
    double *ptr = (double *)(matrix + rows);

    for (int i = 0; i < rows; i++) matrix[i] = ptr + columns * i;
  }
  return matrix;
}

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int err = 0;
  if (rows > 0 && columns > 0 && result != NULL) {
    double **matrix = createMatrix(rows, columns);
    if (matrix != NULL) {
      result->matrix = matrix;
      result->rows = rows;
      result->columns = columns;
    } else
      err = 1;
  } else
    err = 1;
  return err;
}

void s21_initialize_matrix(matrix_t *A, double start_value,
                           double iteration_step) {
  if (A != NULL && A->matrix != NULL) {
    double value = start_value;
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        A->matrix[i][j] = value;
        value += iteration_step;
      }
    }
  }
}
matrix_t A;

void s21_remove_matrix(matrix_t *A) {
  if (A != 0) {
    // if (A->matrix != NULL) {
    free(A->matrix);
    A->matrix = NULL;
    // }
    A->rows = 0;
    A->columns = 0;
  }
}

int checkAll(matrix_t *A) {
  int err = 0;
  if (A == NULL || A->matrix == NULL)
    err = 1;
  else if (A->rows <= 0 || A->columns <= 0)
    err = 1;
  return err;
}

int checkDimensionAndNullMatrix(matrix_t *A, matrix_t *B) {
  int err = 0;
  int errA = checkAll(A);
  int errB = checkAll(B);
  if (errA != 0)
    err = errA;
  else if (errB != 0)
    err = errB;
  else if ((A->columns != B->columns) || (A->rows != B->rows))
    err = 2;

  return err;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int err = checkDimensionAndNullMatrix(A, B);
  if (err == 0) {
    double temp;
    for (int i = 0; i < A->rows && err == 0; i++)
      for (int j = 0; j < A->columns; j++) {
        temp = fabs(A->matrix[i][j] - B->matrix[i][j]);
        if (temp >= EXP) err = 1;
      }
  }
  err = (err == 0) ? 1 : 0;
  return err;
}

int checkForArifmetic(matrix_t *A, matrix_t *B, matrix_t *result) {
  int err = 0;
  int err2 = checkDimensionAndNullMatrix(A, B);
  if (err2 != 0)
    err = err2;
  else if (result == NULL)
    err = 1;
  return err;
}

int SumSubMatrix(matrix_t *A, matrix_t *B, matrix_t *result, int plus) {
  int err = checkForArifmetic(A, B, result);
  if (err == 0) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        if (plus != 0)
          result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
        else
          result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
  }
  return err;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int err = SumSubMatrix(A, B, result, 1);
  return err;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int err = SumSubMatrix(A, B, result, 0);
  return err;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int err = 0;
  int errTemp = checkAll(A);
  if (errTemp != 0)
    err = errTemp;
  else if (isnan(number) || isinf(number))
    err = 2;
  else if (result == NULL)
    err = 1;
  else {
    s21_create_matrix(A->rows, A->columns, result);

    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        result->matrix[i][j] = A->matrix[i][j] * number;
  }
  return err;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int err = 0;
  int errA = checkAll(A);
  int errB = checkAll(B);
  if (errA != 0)
    err = errA;
  else if (errB != 0)
    err = errB;
  else if ((A->columns != B->rows))
    err = 2;
  else if (result == NULL)
    err = 1;
  else {
    int j;
    int k;
    double var;
    s21_create_matrix(A->rows, B->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (j = 0; j < B->columns; j++) {
        var = 0;
        for (k = 0; k < B->rows; k++) var += A->matrix[i][k] * B->matrix[k][j];
        result->matrix[i][j] = var;
      }
    }
  }
  return err;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int err = 0;
  int errA = checkAll(A);
  if (errA != 0)
    err = errA;
  else if (result == NULL)
    err = 1;
  else {
    s21_create_matrix(A->columns, A->rows, result);

    for (int i = 0; i < A->columns; i++)
      for (int j = 0; j < A->rows; j++) result->matrix[i][j] = A->matrix[j][i];
  }
  return err;
}

int checkForLast3Func(matrix_t *A, void *result) {
  int err = 0;
  int errA = checkAll(A);
  if (errA != 0)
    err = errA;
  else if (A->columns != A->rows)
    err = 2;
  else if (result == NULL)
    err = 1;
  return err;
}

double sign(int i) { return (i % 2 == 0) ? 1 : -1; }

matrix_t getMinor(matrix_t *A, int row, int column) {
  matrix_t B;
  s21_create_matrix(A->rows - 1, A->rows - 1, &B);
  int n = A->columns;
  int ii = 0;
  int jj;
  for (int i = 0; i < n; i++) {
    jj = 0;
    for (int j = 0; j < n; j++) {
      if (j != column && i != row) {
        B.matrix[ii][jj++] = A->matrix[i][j];
      }
    }
    if (i != row) ii++;
  }
  return B;
}

double findDeterminand(matrix_t *A) {
  double determinant = 0;
  if (A->columns == 2)
    determinant =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
  else {
    for (int i = 0; i < A->columns; i++) {
      matrix_t B = getMinor(A, 0, i);
      determinant += sign(i) * A->matrix[0][i] * findDeterminand(&B);
      s21_remove_matrix(&B);
    }
  }
  return determinant;
}

int s21_determinant(matrix_t *A, double *result) {
  int err = 0;
  int errAll = checkForLast3Func(A, result);
  if (errAll != 0)
    err = errAll;
  else if (A->columns == 1)
    *result = A->matrix[0][0];
  else
    *result = findDeterminand(A);
  return err;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int err = 0;
  int errAll = checkForLast3Func(A, result);
  if (errAll != 0)
    err = errAll;
  else if (A->rows == 1) {
    s21_create_matrix(A->rows, A->columns, result);
    result->matrix[0][0] = 1;
  } else {
    int n = A->columns;
    double determinant;
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        matrix_t B = getMinor(A, i, j);
        s21_determinant(&B, &determinant);
        result->matrix[i][j] = sign(i + j) * determinant;
        s21_remove_matrix(&B);
      }
    }
  }
  return err;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int err = 0;
  int errAll = checkForLast3Func(A, result);
  if (errAll != 0)
    err = errAll;
  else {
    double determinant;
    s21_determinant(A, &determinant);
    if (fabs(determinant) >= EXP) {
      matrix_t calcComplementsMatrix;
      s21_calc_complements(A, &calcComplementsMatrix);

      matrix_t transposeMatrix;
      s21_transpose(&calcComplementsMatrix, &transposeMatrix);

      s21_mult_number(&transposeMatrix, 1 / determinant, result);

      s21_remove_matrix(&calcComplementsMatrix);
      s21_remove_matrix(&transposeMatrix);

    } else
      err = 2;
  }
  return err;
}
