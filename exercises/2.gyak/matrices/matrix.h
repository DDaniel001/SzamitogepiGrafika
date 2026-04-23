#ifndef MATRIX_H
#define MATRIX_H


void init_zeros_matrix(float matrix[3][3]);

void init_identity_matrix(float matrix[3][3]);

void multiply_matrix_by_scalar(float matrix[3][3], float scalar);

void multiply_matrices(const float a[3][3], const float b[3][3], float c[3][3]);

void transform_point(const float matrix[3][3], const float point[3], float result[3]);

void scale(float matrix[3][3], float sx, float sy);
void shift(float matrix[3][3], float dx, float dy);
void rotate(float matrix[3][3], float angle_deg);



/**
 * Print the elements of the matrix.
 */
void print_matrix(const float matrix[3][3]);

/**
 * Add matrices.
 */
void add_matrices(const float a[3][3], const float b[3][3], float c[3][3]);

#endif // MATRIX_H

