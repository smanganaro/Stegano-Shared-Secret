#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdint.h>
#include <inttypes.h>

typedef struct
{
  uint32_t rows;
  uint32_t cols;
  uint32_t values[];
} matrix_t;

/**
 * @brief Perform m1+m2
 *
 * @param m1
 * @param m2
 * @return matrix_t* a new matrix with the result of the operation
 */
matrix_t*
matrix_sum(matrix_t* m1, matrix_t* m2);

/**
 * @brief Perform m1-m2
 *
 * @param m1
 * @param m2
 * @return matrix_t* a new matrix with the result of the operation
 */
matrix_t*
matrix_sub(matrix_t* m1, matrix_t* m2);

/**
 * @brief Perform m1*m2
 *
 * @param m1
 * @param m2
 * @return matrix_t* a new matrix with the result of the operation
 */
matrix_t*
matrix_mul(matrix_t* m1, matrix_t* m2);

/**
 * @brief Perform e*m
 *
 * @param e
 * @param m
 * @return matrix_t* a new matrix with the result of the operation
 */
matrix_t*
matrix_esc_mul(uint32_t esc, matrix_t* m);

/**
 * @brief Transpose m
 *
 * @param m
 * @return matrix_t* a new matrix with m transposed
 */
matrix_t*
matrix_trans(matrix_t* m);


/**
 * @brief Creates a random matrix
 *
 * @param rows
 * @param cols
 * @return matrix_t* a new matrix with all its values random
 */
matrix_t*
random_matrix(uint32_t rows, uint32_t cols);

void print_matrix(matrix_t * matrix);

matrix_t * ones(uint32_t rows, uint32_t cols);

matrix_t * merge(matrix_t * m1, matrix_t * m2);

matrix_t * inverse(matrix_t * m);

matrix_t * projection(matrix_t * m);

int rank(matrix_t * m);
#endif // MATRIX_H_