#include "scene.h"
#include <GL/gl.h>
#include <stdio.h>

void init_scene(Scene* scene) {
    glEnable(GL_TEXTURE_2D);

    scene->floor_texture = load_texture("assets/textures/floor.png");
    
    if (scene->floor_texture == 0) {
        printf("Error: Failed to load floor texture!\n");
    }
}

void update_scene(Scene* scene, double time_step) {
    (void)scene;
    (void)time_step;
}

void render_scene(const Scene* scene) {
    glColor3f(1.0f, 1.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, scene->floor_texture);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);   glVertex3f(-10.0f, 0.0f, -10.0f);
        glTexCoord2f(10.0f, 0.0f);  glVertex3f( 10.0f, 0.0f, -10.0f);
        glTexCoord2f(10.0f, 10.0f); glVertex3f( 10.0f, 0.0f,  10.0f);
        glTexCoord2f(0.0f, 10.0f);  glVertex3f(-10.0f, 0.0f,  10.0f);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);
}