#ifndef _GAUSS_H_
#define _GAUSS_H_

#include <stdio.h>
#include <string.h>
#include "mod.h"
#include "matrix.h"
#include "matrixOp.h"

/**
 * Finds columns with nonzero elems on it
 * stops at first result nonzero element found
 */
uint8_t find_non_zero(const matrix_t* matrix, uint32_t starting_row, uint32_t column, uint32_t* ans);

/**
 * swap two rows
 */
void swap_rows(matrix_t* matrix, uint32_t row1, uint32_t row2);
/**
 * reduction using Gauss Jordan Method
 */
matrix_t* gauss_reduction(const matrix_t* matrix);

#ifndef end