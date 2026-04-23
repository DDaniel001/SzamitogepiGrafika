#include <stdio.h>
#include "matrix.h"

// Identitásmátrix létrehozása
Matrix identity_matrix() {
    Matrix m = {0};
    for (int i = 0; i < SIZE; i++) {
        m.data[i][i] = 1.0;
    }
    return m;
}

// Mátrix kiíratása
void print_matrix(Matrix m) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%6.2f ", m.data[i][j]);
        }
        printf("\n");
    }
}

// Transzformáció alkalmazása (eltolás)
void apply_transformation(Matrix* m, float tx, float ty) {
    m->data[0][2] += tx;
    m->data[1][2] += ty;
}
