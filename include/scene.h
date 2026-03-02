#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"
#include <obj/model.h>


typedef struct BoundingBox {
    float min_x, max_x;
    float min_z, max_z;
} BoundingBox;

/**
 * Scene structure holding textures, models, and lighting information.
 */
typedef struct Scene {
    GLuint floor_texture;
    GLuint wall_texture;
    GLuint ceiling_texture;
    Model anvil;
    GLuint anvil_texture;
    Model sword;
    GLuint sword_texture;
    float sword_rotation;
    GLuint help_texture;
    float light_intensity;
    int show_help;

    BoundingBox boxes[10];
    int box_count;
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
void update_scene(Scene* scene, double time_step, float player_x, float player_z);

/**
 * Renders the scene's 3D models and applies textures and lighting.
 * @param scene Pointer to the scene structure.
 */
void render_scene(const Scene* scene);

bool check_collision(const Scene* scene, float x, float z);

#endif