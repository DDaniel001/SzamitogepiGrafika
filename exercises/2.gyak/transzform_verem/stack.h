#ifndef STACK_H
#define STACK_H

#include "matrix.h"

// Verem csomópontja
typedef struct Node {
    Matrix matrix;
    struct Node* next;
} Node;

// Verem struktúra
typedef struct {
    Node* top;
} Stack;

// Verem inicializálása
void init_stack(Stack* stack);

// Mátrix hozzáadása a veremhez
void push_matrix(Stack* stack, Matrix matrix);

// Mátrix kivétele a veremből
Matrix pop_matrix(Stack* stack);

// Verem felszabadítása
void free_stack(Stack* stack);

#endif
