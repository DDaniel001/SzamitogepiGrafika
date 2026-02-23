#include "scene.h"
#include <GL/gl.h>
#include <stdio.h>
#include <obj/load.h>
#include <obj/draw.h>

void init_scene(Scene* scene) {
    glEnable(GL_TEXTURE_2D);

    /* Initialize light intensity to default */
    scene->light_intensity = 1.0f;

    /* Setup lighting */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    /* Setup initial light properties */
    GLfloat ambient_light[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat diffuse_light[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specular_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);

    /* Load textures and models */
    scene->floor_texture = load_texture("assets/textures/floor.png");    
    if (scene->floor_texture == 0) {
        printf("Error: Failed to load floor texture!\n");
    }

    scene->anvil_texture = load_texture("assets/textures/anvil.jpg");
    if (scene->anvil_texture == 0) {
        printf("Error: Failed to load anvil texture!\n");
    }

    if (!load_model(&(scene->anvil), "assets/models/anvil.obj")) {
        printf("Error: Failed to load anvil model!\n");
    }

    /* Initialize help overlay */
    scene->show_help = 0;
    scene->help_texture = load_texture("assets/textures/help.png");
}

void update_scene(Scene* scene, double time_step) {
    (void)scene;
    (void)time_step;
}

/* Helper function to draw the help overlay */
void draw_help(GLuint texture_id) {
    /* Get current window dimensions from viewport */
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int width = viewport[2];
    int height = viewport[3];

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    /* Use pixel-based coordinate system to prevent distortion */
    glOrtho(0, width, height, 0, -1, 1); 

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    /* Set help window size and center it */
    float helpW = 600.0f;
    float helpH = 450.0f;

    /* Scale help window if the actual window is too small, maintaining aspect ratio */
    if (helpW > width * 0.9f) {
        helpW = width * 0.9f;
        helpH = helpW * 0.75f;
    }
    if (helpH > height * 0.9f) {
        helpH = height * 0.9f;
        helpW = helpH * 1.333f;
    }

    float x1 = (width - helpW) / 2.0f;
    float y1 = (height - helpH) / 2.0f;
    float x2 = x1 + helpW;
    float y2 = y1 + helpH;

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(x1, y1);
        glTexCoord2f(1, 0); glVertex2f(x2, y1);
        glTexCoord2f(1, 1); glVertex2f(x2, y2);
        glTexCoord2f(0, 1); glVertex2f(x1, y2);
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void render_scene(const Scene* scene) {
    /* Get current intensity */
    float i = scene->light_intensity;

    /* Update light components based on intensity. 
       Keep ambient light constant to avoid "monitor brightness" effect. */
    GLfloat ambient_light[]  = { 0.15f, 0.15f, 0.15f, 1.0f }; 
    GLfloat diffuse_light[]  = { 0.8f * i, 0.8f * i, 0.8f * i, 1.0f };
    GLfloat specular_light[] = { 1.0f * i, 1.0f * i, 1.0f * i, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);

    /* Set light position */
    GLfloat light_position[] = { 0.0f, 4.0f, -1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glColor3f(1.0f, 1.0f, 1.0f);

    /* 1. Render floor */
    glBindTexture(GL_TEXTURE_2D, scene->floor_texture);
    glNormal3f(0.0f, 1.0f, 0.0f); 

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);   glVertex3f(-10.0f, 0.0f, -10.0f);
        glTexCoord2f(10.0f, 0.0f);  glVertex3f( 10.0f, 0.0f, -10.0f);
        glTexCoord2f(10.0f, 10.0f); glVertex3f( 10.0f, 0.0f,  10.0f);
        glTexCoord2f(0.0f, 10.0f);  glVertex3f(-10.0f, 0.0f,  10.0f);
    glEnd();

    /* 2. Render anvil */
    glPushMatrix();
        glTranslatef(0.0f, -0.4f, -3.0f);         
        glRotatef(0.0f, 1.0f, 0.0f, 0.0f); 
        glScalef(0.4f, 0.4f, 0.4f);

        /* Set material for metallic shininess */
        GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);

        glBindTexture(GL_TEXTURE_2D, scene->anvil_texture);        
        draw_model(&(scene->anvil));
        
        /* Reset specular material to avoid shiny floor */
        GLfloat default_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_SPECULAR, default_specular);
    glPopMatrix();
    
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Draw help overlay if enabled */
    if (scene->show_help) {
        draw_help(scene->help_texture);
    }
}