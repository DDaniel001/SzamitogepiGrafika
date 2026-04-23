#include <stdio.h>
#include "matrix.h"
#include "stack.h"

int main() {
    Stack stack;
    init_stack(&stack);

    // Kezdő mátrix (identitásmátrix)
    Matrix current_matrix = identity_matrix();
    printf("Kezdo matrix:\n");
    print_matrix(current_matrix);

    // Első transzformáció elmentése
    push_matrix(&stack, current_matrix);
    apply_transformation(&current_matrix, 5.0, 3.0);
    printf("\nTranszformacio utan:\n");
    print_matrix(current_matrix);

    // Második transzformáció elmentése
    push_matrix(&stack, current_matrix);
    apply_transformation(&current_matrix, -2.0, 4.0);
    printf("\nMasodik transzformacio utan:\n");
    print_matrix(current_matrix);

    // Visszavonás
    current_matrix = pop_matrix(&stack);
    printf("\nVisszavonas utan:\n");
    print_matrix(current_matrix);

    current_matrix = pop_matrix(&stack);
    printf("\nMasodik visszavonas utan (vissza az eredetihez):\n");
    print_matrix(current_matrix);

    // Memória felszabadítása
    free_stack(&stack);
    
    return 0;
}
