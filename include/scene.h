#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"

/**
 * Scene structure holding textures, models, and lighting information.
 */
typedef struct Scene {
    GLuint floor_texture;
} Scene;

/**
 * Initializes the scene, loads textures and models.
 * @param scene Pointer to the scene structure.
 */
void init_scene(Scene* scene);

/**
 * Updates scene elements (e.g., animations) based on the elapsed time.
 * @param scene Pointer to the scene structure.
 * @param time_step The elapsed time since the last frame (in seconds).
 */
void update_scene(Scene* scene, double time_step);

/**
 * Renders the scene's 3D models and applies textures and lighting.
 * @param scene Pointer to the scene structure.
 */
void render_scene(const Scene* scene);

#endif