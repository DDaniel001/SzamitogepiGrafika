#include "matrix.h"
#include <math.h>
#include <stdio.h>


void init_zeros_matrix(float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = 0.0;
        }
    }
}

void init_identity_matrix(float matrix[3][3])
{
    int i, j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

void multiply_matrix_by_scalar(float matrix[3][3], float scalar)
{
    int i, j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] *= scalar;
        }
    }
}

void multiply_matrices(const float a[3][3], const float b[3][3], float c[3][3])
{
    int i, j, k;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            c[i][j] = 0.0f;
            for (k = 0; k < 3; ++k) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void transform_point(const float matrix[3][3], const float point[3], float result[3])
{
    int i, j;
    for (i = 0; i < 3; ++i) {
        result[i] = 0.0f;
        for (j = 0; j < 3; ++j) {
            result[i] += matrix[i][j] * point[j];
        }
    }
}

void scale(float matrix[3][3], float sx, float sy)
{
    matrix[0][0] *= sx;  // x tengely mentén történő skálázás
    matrix[1][1] *= sy;  // y tengely mentén történő skálázás
}

void shift(float matrix[3][3], float dx, float dy)
{
    matrix[0][2] += dx;  // x irányú eltolás
    matrix[1][2] += dy;  // y irányú eltolás
}

void rotate(float matrix[3][3], float angle_deg)
{
    float angle_rad = angle_deg * (M_PI / 180.0f);  // Átváltás radiánba
    float cos_theta = cos(angle_rad);
    float sin_theta = sin(angle_rad);

    matrix[0][0] = cos_theta;  // x tengely
    matrix[0][1] = -sin_theta; // y tengely
    matrix[1][0] = sin_theta;  // x tengely
    matrix[1][1] = cos_theta;  // y tengely
}



void print_matrix(const float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            printf("%4.4f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const float a[3][3], const float b[3][3], float c[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

