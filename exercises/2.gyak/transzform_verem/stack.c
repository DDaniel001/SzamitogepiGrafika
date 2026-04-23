#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

// Verem inicializálása
void init_stack(Stack* stack) {
    stack->top = NULL;
}

// Mátrix hozzáadása a veremhez
void push_matrix(Stack* stack, Matrix matrix) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        printf("Memoriafoglalasi hiba!\n");
        exit(1);
    }
    new_node->matrix = matrix;
    new_node->next = stack->top;
    stack->top = new_node;
}

// Mátrix kivétele a veremből
Matrix pop_matrix(Stack* stack) {
    if (!stack->top) {
        printf("Hiba: Üres verem!\n");
        exit(1);
    }
    Node* temp = stack->top;
    Matrix matrix = temp->matrix;
    stack->top = temp->next;
    free(temp);
    return matrix;
}

// Verem felszabadítása
void free_stack(Stack* stack) {
    while (stack->top) {
        pop_matrix(stack);
    }
}
