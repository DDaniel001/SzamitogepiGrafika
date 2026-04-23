#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>

// One pixel on 3 bytes (RGB)
typedef GLubyte Pixel[3];

/**
 * Loads an image file (JPG, PNG) and returns the OpenGL texture ID.
 * @param filename The path of the image file.
 * @return The OpenGL texture ID (GLuint), or 0 on error.
 */
GLuint load_texture(const char* filename);

#endif