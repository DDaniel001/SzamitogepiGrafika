#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "camera.h"
#include "scene.h"

/**
 * Application state structure.
 */
typedef struct App {
    SDL_Window* window;
    SDL_GLContext gl_context;
    bool is_running;
    bool is_mouse_captured;
    
    int window_width;
    int window_height;
    
    Uint32 last_time; // Time calculation (Delta Time)
    
    Camera camera;
    Scene scene;
} App;

/**
 * Initializes the application, creates the window and sets up the OpenGL context.
 * @param app Pointer to the application state.
 * @param width Width of the window.
 * @param height Height of the window.
 * @param title Title of the window.
 * @return True on success, false on failure.
 */
bool init_app(App* app, int width, int height, const char* title);

/**
 * Handles user input events (keyboard, mouse, window events).
 * @param app Pointer to the application state.
 */
void handle_input(App* app);

/**
 * Updates the application state based on the elapsed time.
 * @param app Pointer to the application state.
 */
void update_app(App* app);

/**
 * Renders the 3D scene to the screen.
 * @param app Pointer to the application state.
 */
void render_app(App* app);

/**
 * Destroys the application and frees allocated resources.
 * @param app Pointer to the application state.
 */
void destroy_app(App* app);

#endif