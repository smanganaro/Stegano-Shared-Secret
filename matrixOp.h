#ifndef _MATRIX_OP_H
#define _MATRIX_OP_H

#include <stdio.h>
#include <stdlib.h>
#include "stego.h"
#include <string.h>
#include "mod.h"

#define MAX 50

/**
 * Switch row values for column values, switch row amount for col amount
 */
matrix_t* matrix_transpose(matrix_t* m) ;

/**
 * Create a random matrix using int values
 */
matrix_t* random_matrix(uint32_t rows, uint32_t cols, uint32_t range);

void print_matrix(matrix_t * matrix);

/**
 * Create an identity matrix
 */
matrix_t * id_matrix(uint32_t rows, uint32_t cols) ;

/**
 * Join two matrices by cols
 */
matrix_t * merge(matrix_t * m1, matrix_t * m2) ;

/**
 * Find inverse using Gauss Jordan method
 */
matrix_t * inverse(matrix_t * m);

/**
 * Calculate matrix projection
 */
matrix_t * projection(matrix_t * m);

/**
 * The rank of a matrix is the amount of columns that are linearly independent
 */
int rank(matrix_t * m) ;


/**
 * Switch row values for column values, switch row amount for col amount
 */
matrix_t* matrix_mul(matrix_t* m1, matrix_t* m2);

/**
 * Applies operations between matrices
 */
matrix_t* matrix_binary_op(matrix_t* m1, matrix_t* m2,
                                  uint32_t (*op)(uint32_t, uint32_t));

matrix_t* matrix_sum(matrix_t* m1, matrix_t* m2);

matrix_t* matrix_sub(matrix_t* m1, matrix_t* m2);

matrix_t* matrix_mul_esc(uint32_t e, matrix_t* m);

#ifndef end
