#include <stdio.h>
#include <stdlib.h>
#include "stego.h"
#include <string.h>
#include "mod.h"
#include "matrixOp.h"

/**
 * Switch row values for column values, switch row amount for col amount
 */
matrix_t* matrix_transpose(matrix_t* m) {

  matrix_t* ans = NEW_MATRIX(m->cols, m->rows);
  ans->rows     = m->cols;
  ans->cols     = m->rows;

  for (uint32_t row = 0; row < m->rows; row++) {
    for (uint32_t col = 0; col < m->cols; col++) {
      ans->values[col * ans->cols + row] = m->values[row * m->cols + col];
    }
  }

  return ans;
}

/**
 * Create a random matrix using int values
 */
matrix_t* random_matrix(uint32_t rows, uint32_t cols, uint32_t range){
    int i, j;
    matrix_t * m = NEW_MATRIX(rows, cols);

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            m->values[i*cols + j] = rand()%range;
        }
    }
    m->rows = rows;
    m->cols = cols;
    return m;
}

void print_matrix(matrix_t * matrix) {
    for(int i = 0; i < matrix->rows; i++) {
        for(int j = 0; j < matrix->cols; j++) {
            printf("%ld", ELEM(matrix, i, j));
        }
        putchar('\n');
    }
}

/**
 * Create an identity matrix
 */
matrix_t * id_matrix(uint32_t rows, uint32_t cols) {
    matrix_t * m = NEW_MATRIX(rows, cols);
    m->rows = rows;
    m->cols = cols;

    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->cols; j++) {
            if(i == j)
                 m->values[i*cols + j] = 1;
            else
                m->values[i*cols + j] = 0;
        }
    }
    return m;
}

/**
 * Join two matrices by cols
 */
matrix_t * merge(matrix_t * m1, matrix_t * m2) {
    matrix_t * m = NEW_MATRIX(m1->rows, m1->cols + m2->cols);
    m->rows = m1->rows;
    m->cols = m1->cols + m2->cols;
    uint32_t * aux_m = m->values;
    for(int i = 0; i < m1->rows; i++) {
        memcpy(aux_m, m1->values + i * m1->cols, m1->cols*sizeof(*m1->values));
        aux_m += m1->cols;
        memcpy(aux_m, m2->values + i * m2->cols, m2->cols*sizeof(*m2->values));
        aux_m += m2->cols;

    }

    return m;
}

/**
 * Find inverse using Gauss Jordan method
 */
matrix_t * inverse(matrix_t * m) {
	// Extend matrix with identity -> (left=matrix to inverse|right=id matrix)
    matrix_t * extended = merge(m, id_matrix(m->rows, m->cols));
    // Reduction -> get id matrix on left side
    matrix_t * gauss = gauss_reduction(extended);
    matrix_t * inverse = NEW_MATRIX(m->rows, gauss->cols / 2);
    inverse->rows = m->rows;
    inverse->cols = gauss->cols / 2;
    uint32_t * aux_m = inverse->values;
    // Copy right matrix to inverse matrix
    for(int i=0; i<gauss->rows ; i++){
    	for(int j=0; j<gauss->cols ;j++){
    		if(j > m->cols){
    			memcpy(aux_m, gauss->values + i * j, sizeof(uint32_t));
                aux_m += sizeof(uint32_t);
    		}
    	}
    }

    /*
    for(int i = 1; (i * m->cols) < gauss->rows * gauss->cols; i++) {
            if(i % 2 == 1) {
                memcpy(aux_m, gauss->values + m->cols * i, m->cols * sizeof(*m->values));
                aux_m += m->cols;
            }
    }
    */
    return inverse;
}

/**
 * Calculate matrix projection
 */
matrix_t * projection(matrix_t * m) {
    matrix_t * transposed = matrix_transpose(m); // At
    matrix_t * a = matrix_mul(transposed,m);// At.A
    matrix_t * b = inverse(a); // (At.A)^(-1)
    matrix_t * c = matrix_mul(m, b); 
    matrix_t * d = matrix_mul(c, transposed);
    return d;
}

/**
 * The rank of a matrix is the amount of columns that are linearly independent
 */
int rank(matrix_t * m) {
    matrix_t * gauss = gauss_reduction(mod_matrix_transpose(m));
    int zero_rows = 0;
    int count = 0;
    for (int row = 0; row < gauss->rows; row++) {

        count = 0;
        for(int col = 0; col < gauss->cols; col++) {
            if (ELEM(gauss, row, col) == 0) {
                count++;
            } else {
                count = 0;
            }
        }
        if(count == gauss->cols) {
            zero_rows++;
        }

    }
    return m->cols - zero_rows;
}


/**
 * Switch row values for column values, switch row amount for col amount
 */
matrix_t* matrix_mul(matrix_t* m1, matrix_t* m2){
  if (m1->cols != m2->rows) {
    fprintf(stderr, "Inner matrix dimensions must match!");
    exit(EXIT_FAILURE);
  }

  matrix_t* ans = NEW_MATRIX(m1->rows, m2->cols);
  ans->rows     = m1->rows;
  ans->cols     = m2->cols;

  /** Transpose m2 to use memory secuentially */
  matrix_t* m2_tr = matrix_transpose(m2);

  uint32_t inner_dim = m1->cols;
  uint32_t sum;

  // iterate rows of m1
  for (uint32_t row = 0; row < ans->rows; row++) {
  	// iterate cols of m2
    for (uint32_t col = 0; col < ans->cols; col++) {
      sum = 0;

      for (uint32_t k = 0; k < inner_dim; k++) {
        sum = mod_sum(sum,
                      mod_mul(m1->values[row * m1->cols + k],
                              m2_tr->values[col * m2_tr->cols + k]));
      }
      ans->values[row * ans->cols + col] = sum;
    }
  }

  /** Free m2_tr as it won't be used */
  free(m2_tr);

  return ans;
}

/**
 * Applies operations between matrices
 */
matrix_t* matrix_binary_op(matrix_t* m1, matrix_t* m2,
                                  uint32_t (*op)(uint32_t, uint32_t)){

  	if (m1->cols != m2->cols || m1->rows != m2->rows){
    	fprintf(stderr, "Matrix sizes must match!");
    	exit(EXIT_FAILURE);
  	}

  	matrix_t* ans = NEW_MATRIX(m1->cols, m1->rows);
  	ans->rows = m1->rows;
  	ans->cols = m1->cols;

  	uint32_t* ans_p = ans->values;
  	uint32_t* m1_p = m1->values;
  	uint32_t* m2_p = m2->values;
  	uint32_t i = ans->cols * ans->rows;

  while (i--) {
    *(ans_p++) = op(*(m1_p++), *(m2_p++));
  }

  return ans;
}

matrix_t* matrix_sum(matrix_t* m1, matrix_t* m2) {
  return matrix_binary_op(m1, m2, mod_sum);
}

matrix_t* matrix_sub(matrix_t* m1, matrix_t* m2)
{
  return matrix_binary_op(m1, m2, mod_sub);
}

matrix_t* matrix_mul_esc(uint32_t e, matrix_t* m) {

    matrix_t* ans = NEW_MATRIX(m->cols, m->rows);
    ans->rows = m->cols;
    ans->cols = m->rows;
    int i, j = 0;
    for(i = 0; i < m->rows; i++) {
        for (j = 0; j < m->cols; j++) {
            ans->values[i*ans->cols + j] = mod_mul(m->values[i*m->cols + j], e);
        }
    }
    return ans;
}

