#ifndef MATRIX_H
#define MATRIX_H

#define SIZE 3

// Mátrix struktúra
typedef struct {
    float data[SIZE][SIZE];
} Matrix;

// Mátrix inicializálása identitásmátrixként
Matrix identity_matrix();

// Mátrix kiíratása
void print_matrix(Matrix m);

// Transzformáció alkalmazása (eltolás)
void apply_transformation(Matrix* m, float tx, float ty);

#endif
