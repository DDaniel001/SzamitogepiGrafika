#ifndef CAMERA_H
#define CAMERA_H

#include <stdbool.h>

/**
 * Camera structure for handling 3D viewing and movement.
 */
typedef struct Camera {
    float x, y, z;
    float rotation_x;
    float rotation_y;
    float speed;
    
    bool is_moving_forward;
    bool is_moving_backward;
    bool is_stepping_left;
    bool is_stepping_right;
} Camera;

/**
 * Initializes the camera with default position and parameters.
 * @param camera Pointer to the camera structure.
 */
void init_camera(Camera* camera);

/**
 * Updates the camera's position based on movement flags and elapsed time.
 * @param camera Pointer to the camera structure.
 * @param time_step The elapsed time since the last frame (in seconds).
 */
void update_camera(Camera* camera, double time_step);

/**
 * Applies the camera's viewing transformation using OpenGL commands.
 * @param camera Pointer to the camera structure.
 */
void set_view(const Camera* camera);

/**
 * Sets the projection matrix based on the window's aspect ratio.
 * @param width Current width of the window.
 * @param height Current height of the window.
 */
void set_projection(int width, int height);

#endif