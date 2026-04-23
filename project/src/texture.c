#include "texture.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

GLuint load_texture(const char* filename) {
    SDL_Surface* loaded_image = IMG_Load(filename);
    if (loaded_image == NULL) {
        printf("Error loading texture (%s): %s\n", filename, IMG_GetError());
        return 0;
    }

    SDL_Surface* formatted_image = SDL_ConvertSurfaceFormat(loaded_image, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(loaded_image);

    if (formatted_image == NULL) {
        printf("Error converting texture (%s): %s\n", filename, SDL_GetError());
        return 0;
    }

    GLuint texture_id = 0;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        GL_RGBA, 
        formatted_image->w, 
        formatted_image->h, 
        0, 
        GL_RGBA, 
        GL_UNSIGNED_BYTE, 
        formatted_image->pixels
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(formatted_image);

    return texture_id;
}