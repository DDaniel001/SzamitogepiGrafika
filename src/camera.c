#include "camera.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void init_camera(Camera* camera) {
    camera->x = 0.0f;
    camera->y = 1.5f; 
    camera->z = 5.0f; 
    camera->rotation_x = 0.0f;
    camera->rotation_y = 0.0f;
    camera->speed = 3.0f; 

    camera->is_moving_forward = false;
    camera->is_moving_backward = false;
    camera->is_stepping_left = false;
    camera->is_stepping_right = false;
}

void update_camera(Camera* camera, double time_step) {
    float distance = camera->speed * time_step;
    float angle = camera->rotation_y * M_PI / 180.0f;

    if (camera->is_moving_forward) {
        camera->x += sin(angle) * distance;
        camera->z -= cos(angle) * distance;
    }
    if (camera->is_moving_backward) {
        camera->x -= sin(angle) * distance;
        camera->z += cos(angle) * distance;
    }
    if (camera->is_stepping_left) {
        camera->x -= cos(angle) * distance;
        camera->z -= sin(angle) * distance;
    }
    if (camera->is_stepping_right) {
        camera->x += cos(angle) * distance;
        camera->z += sin(angle) * distance;
    }
}

void set_view(const Camera* camera) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(camera->rotation_x, 1.0f, 0.0f, 0.0f);
    glRotatef(camera->rotation_y, 0.0f, 1.0f, 0.0f);

    glTranslatef(-camera->x, -camera->y, -camera->z);
}

void set_projection(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (height == 0) height = 1; 

    double aspect_ratio = (double)width / (double)height;
    
    gluPerspective(60.0, aspect_ratio, 0.1, 100.0); 
}