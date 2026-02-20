#include "app.h"
#include <GL/gl.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>

bool init_app(App* app, int width, int height, const char* title) {
    app->is_running = false;
    app->window_width = width;
    app->window_height = height;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization error: %s\n", SDL_GetError());
        return false;
    }

    int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(img_flags) & img_flags)) {
        printf("SDL_image initialization error: %s\n", IMG_GetError());
        SDL_Quit();
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    app->window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        app->window_width,
        app->window_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (app->window == NULL) {
        printf("Window creation error: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL) {
        printf("OpenGL context error: %s\n", SDL_GetError());
        SDL_DestroyWindow(app->window);
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    init_camera(&(app->camera));
    init_scene(&(app->scene));
    
    SDL_SetRelativeMouseMode(SDL_TRUE);
    app->last_time = SDL_GetTicks();
    set_projection(app->window_width, app->window_height);

    app->is_running = true;
    return true;
}

void handle_input(App* app) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                app->is_running = false;
                break;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    app->window_width = event.window.data1;
                    app->window_height = event.window.data2;
                    glViewport(0, 0, app->window_width, app->window_height);
                    set_projection(app->window_width, app->window_height);
                }
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: app->is_running = false; break;
                    case SDLK_w: app->camera.is_moving_forward = true; break;
                    case SDLK_s: app->camera.is_moving_backward = true; break;
                    case SDLK_a: app->camera.is_stepping_left = true; break;
                    case SDLK_d: app->camera.is_stepping_right = true; break;

                    /* Light intensity control */
                    case SDLK_KP_PLUS:
                    app->scene.light_intensity += 0.1f;
                    if (app->scene.light_intensity > 2.0f) app->scene.light_intensity = 2.0f;
                    break;
                    case SDLK_KP_MINUS:
                    app->scene.light_intensity -= 0.1f;
                    if (app->scene.light_intensity < 0.0f) app->scene.light_intensity = 0.0f;
                    break;
                }
                break;

            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_w: app->camera.is_moving_forward = false; break;
                    case SDLK_s: app->camera.is_moving_backward = false; break;
                    case SDLK_a: app->camera.is_stepping_left = false; break;
                    case SDLK_d: app->camera.is_stepping_right = false; break;
                }
                break;
                
            case SDL_MOUSEMOTION:
                app->camera.rotation_y += event.motion.xrel * 0.2f;
                app->camera.rotation_x += event.motion.yrel * 0.2f;
                
                if (app->camera.rotation_x > 90.0f) app->camera.rotation_x = 90.0f;
                if (app->camera.rotation_x < -90.0f) app->camera.rotation_x = -90.0f;
                break;
        }
    }
}

void update_app(App* app) {
    Uint32 current_time = SDL_GetTicks();
    double time_step = (current_time - app->last_time) / 1000.0;
    app->last_time = current_time;

    update_camera(&(app->camera), time_step);
    update_scene(&(app->scene), time_step);
}

void render_app(App* app) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    set_view(&(app->camera));
    render_scene(&(app->scene));

    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App* app) {
    if (app->gl_context) SDL_GL_DeleteContext(app->gl_context);
    if (app->window) SDL_DestroyWindow(app->window);
    
    IMG_Quit();
    SDL_Quit();
}