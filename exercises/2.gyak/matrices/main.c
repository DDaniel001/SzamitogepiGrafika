#include "matrix.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    float a[3][3] = {
        {1.0f, -2.0f, 3.0f},
        {5.0f, -3.0f, 0.0f},
        {-2.0f, 1.0f, -4.0f}};
    float b[3][3];
    float c[3][3];
    float identity[3][3];

    // Inicializálás nullamátrixra
    init_zeros_matrix(b);
    b[1][1] = 8.0f;
    b[2][0] = -3.0f;
    b[2][2] = 5.0f;

    // Egységmátrix inicializálása
    init_identity_matrix(identity);

    // Mátrixok kiírása
    printf("Matrix A:\n");
    print_matrix(a);

    printf("\nMatrix B:\n");
    print_matrix(b);

    printf("\nIdentity Matrix:\n");
    print_matrix(identity);

    // Mátrixok összeadása
    add_matrices(a, b, c);
    printf("\nMatrix A + B:\n");
    print_matrix(c);

    // Mátrix skalárral való szorzása
    multiply_matrix_by_scalar(a, 2.0f);
    printf("\nMatrix A * 2:\n");
    print_matrix(a);

    // Mátrixszorzás
    multiply_matrices(a, b, c);
    printf("\nMatrix A * B:\n");
    print_matrix(c);

    // Egy pont transzformálása a B mátrixszal
    float point[3] = {2.0f, 3.0f, 1.0f}; // Homogén koordinátás pont (x, y, 1)
    float transformed_point[3];

    transform_point(b, point, transformed_point);

    printf("\nTransformed Point:\n");
    printf("x' = %4.4f, y' = %4.4f, w' = %4.4f\n",
           transformed_point[0], transformed_point[1], transformed_point[2]);

    printf("Original Matrix:\n");
    print_matrix(a);

    //Skálázás
    scale(a, 2.0f, 3.0f);
    printf("\nScaled Matrix (sx=2, sy=3):\n");
    print_matrix(a);

    //Eltolás
    shift(a, 5.0f, -3.0f);
    printf("\nShifted Matrix (dx=5, dy=-3):\n");
    print_matrix(a);

    //Forgatás
    rotate(a, 45.0f); // 45 fokkal forgatás
    printf("\nRotated Matrix (angle=45 degrees):\n");
    print_matrix(a);



    return 0;
}
