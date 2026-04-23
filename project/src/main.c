#include "app.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    App app;

    if (init_app(&app, 1024, 768, "Blacksmith OpenGL")) {
        
        while (app.is_running) {
            handle_input(&app);
            update_app(&app);
            render_app(&app);
        }
    } else {
        printf("An error occurred  during initialisation!\n");
        return 1;
    }

    destroy_app(&app);

    return 0;
}