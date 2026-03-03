#include "scene.h"
#include <GL/gl.h>
#include <stdio.h>
#include <obj/load.h>
#include <obj/draw.h>

/* Helper function to add a bounding box to the scene */
void add_box(Scene* scene, float min_x, float max_x, float min_z, float max_z) {
    if (scene->box_count < 10) {
        scene->boxes[scene->box_count].min_x = min_x;
        scene->boxes[scene->box_count].max_x = max_x;
        scene->boxes[scene->box_count].min_z = min_z;
        scene->boxes[scene->box_count].max_z = max_z;
        scene->box_count++;
    }
}

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

    scene->wall_texture = load_texture("assets/textures/wall.jpg");    
    if (scene->wall_texture == 0) {
        printf("Error: Failed to load wall texture!\n");
    }

    scene->ceiling_texture = load_texture("assets/textures/ceiling.jpg");
    if (scene->ceiling_texture == 0) {
        printf("Error: Failed to load ceiling texture!\n");
    }

    scene->anvil_texture = load_texture("assets/textures/anvil.jpg");
    if (scene->anvil_texture == 0) {
        printf("Error: Failed to load anvil texture!\n");
    }

    if (!load_model(&(scene->anvil), "assets/models/anvil.obj")) {
        printf("Error: Failed to load anvil model!\n");
    }

    scene->sword_texture = load_texture("assets/textures/sword.png");
    if (scene->sword_texture == 0) {
        printf("Error: Failed to load sword texture!\n");
    }

    if (!load_model(&(scene->sword), "assets/models/sword.obj")) {
        printf("Error: Failed to load sword model!\n");
    }

    scene->sword_rotation = 0.0f;

    /* Setup collision detection (Bounding boxes) */
    scene->box_count = 0;

    /* 1. Box around the anvil */
    add_box(scene, -0.4f, 0.4f, -4.2f, -1.8f);

    /* 2. Boxes for walls */
    add_box(scene, -11.0f, -9.8f, -10.0f, 10.0f); /* Left wall */
    add_box(scene,  9.8f,  11.0f, -10.0f, 10.0f); /* Right wall */
    add_box(scene, -10.0f, 10.0f, -11.0f, -9.8f); /* Front wall */
    add_box(scene, -10.0f, 10.0f,  9.8f,  11.0f); /* Back wall */

    /* Initialize help overlay */
    scene->show_help = 0;
    scene->help_texture = load_texture("assets/textures/help.png");
}

void update_scene(Scene* scene, double time_step, float player_x, float player_z) {
    /* The sword is positioned at X = 0.0, Z = -3.0 */
    float sword_x = 0.0f;
    float sword_z = -3.0f;
    
    /* The reach of the sword. Adjust this if it stops too early or too late */
    float sword_reach = 2.0f; 

    float dx = player_x - sword_x;
    float dz = player_z - sword_z;
    float distance_squared = (dx * dx) + (dz * dz);

    /* Only rotate the sword if the player is outside its reach */
    if (distance_squared > (sword_reach * sword_reach)) {
        /* Update sword rotation based on time_step */
        scene->sword_rotation += 45.0f * (float)time_step;

        while (scene->sword_rotation > 360.0f) {
            scene->sword_rotation -= 360.0f;
        }
    }
}

bool check_collision(const Scene* scene, float x, float z) {
    /* Check AABB boxes (Anvil, walls) 
       The anvil's box is enough to protect the vertical sword! */
    for (int i = 0; i < scene->box_count; i++) {
        BoundingBox box = scene->boxes[i];
        
        /* If the coordinates are inside the box */
        if (x >= box.min_x && x <= box.max_x &&
            z >= box.min_z && z <= box.max_z) {
            return true; /* Collision detected! */
        }
    }
    return false; /* No collision */
}

/* Helper function to draw the help overlay */
void draw_help(GLuint texture_id) {
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
    glOrtho(0, width, height, 0, -1, 1); 

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    float helpW = 600.0f;
    float helpH = 450.0f;

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
    float i = scene->light_intensity;

    GLfloat ambient_light[]  = { 0.15f, 0.15f, 0.15f, 1.0f }; 
    GLfloat diffuse_light[]  = { 0.8f * i, 0.8f * i, 0.8f * i, 1.0f };
    GLfloat specular_light[] = { 1.0f * i, 1.0f * i, 1.0f * i, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);

    GLfloat light_position[] = { 0.0f, 2.5f, -1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glColor3f(1.0f, 1.0f, 1.0f);

    /* FOG */
    glEnable(GL_FOG);
    
    GLfloat fog_color[] = { 0.15f, 0.15f, 0.15f, 1.0f };
    glFogfv(GL_FOG_COLOR, fog_color);
    
    glFogi(GL_FOG_MODE, GL_EXP);
    
    /* Density */
    glFogf(GL_FOG_DENSITY, 0.08f);
    
    glHint(GL_FOG_HINT, GL_NICEST);

    /* 1. Render floor */
    glBindTexture(GL_TEXTURE_2D, scene->floor_texture);
    glNormal3f(0.0f, 1.0f, 0.0f); 

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);   glVertex3f(-10.0f, 0.0f, -10.0f);
        glTexCoord2f(10.0f, 0.0f);  glVertex3f( 10.0f, 0.0f, -10.0f);
        glTexCoord2f(10.0f, 10.0f); glVertex3f( 10.0f, 0.0f,  10.0f);
        glTexCoord2f(0.0f, 10.0f);  glVertex3f(-10.0f, 0.0f,  10.0f);
    glEnd();
    
    /* 1.2 Render walls */
    glBindTexture(GL_TEXTURE_2D, scene->wall_texture);
    
    glBegin(GL_QUADS);
        /* Front wall (Z = -10.0f) - Normals pointing to +Z */
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f);
        glTexCoord2f(2.0f, 0.0f); glVertex3f( 10.0f, 0.0f, -10.0f);
        glTexCoord2f(2.0f, 1.0f); glVertex3f( 10.0f, 5.0f, -10.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f, 5.0f, -10.0f);

        /* Back wall (Z = 10.0f) - Normals pointing to -Z */
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.0f, 0.0f, 10.0f);
        glTexCoord2f(2.0f, 0.0f); glVertex3f(-10.0f, 0.0f, 10.0f);
        glTexCoord2f(2.0f, 1.0f); glVertex3f(-10.0f, 5.0f, 10.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f, 5.0f, 10.0f);

        /* Left wall (X = -10.0f) - Normals pointing to +X */
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f,  10.0f);
        glTexCoord2f(2.0f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f);
        glTexCoord2f(2.0f, 1.0f); glVertex3f(-10.0f, 5.0f, -10.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f, 5.0f,  10.0f);

        /* Right wall (X = 10.0f) - Normals pointing to -X */
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 0.0f, -10.0f);
        glTexCoord2f(2.0f, 0.0f); glVertex3f(10.0f, 0.0f,  10.0f);
        glTexCoord2f(2.0f, 1.0f); glVertex3f(10.0f, 5.0f,  10.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(10.0f, 5.0f, -10.0f);
    glEnd();

    /* 1.3 Render ceiling */
    glBindTexture(GL_TEXTURE_2D, scene->ceiling_texture); 
    
    glNormal3f(0.0f, -1.0f, 0.0f); 

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 5.0f, -10.0f);
        glTexCoord2f(4.0f, 0.0f); glVertex3f( 10.0f, 5.0f, -10.0f);
        glTexCoord2f(4.0f, 4.0f); glVertex3f( 10.0f, 5.0f,  10.0f);
        glTexCoord2f(0.0f, 4.0f); glVertex3f(-10.0f, 5.0f,  10.0f);
    glEnd();

    /* 2. Render anvil */
    glPushMatrix();
        glTranslatef(0.0f, -0.4f, -3.0f);         
        glRotatef(0.0f, 1.0f, 0.0f, 0.0f); 
        glScalef(0.4f, 0.4f, 0.4f);

        GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);

        glBindTexture(GL_TEXTURE_2D, scene->anvil_texture);        
        draw_model(&(scene->anvil));
        
        GLfloat default_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_SPECULAR, default_specular);
    glPopMatrix();
    
    /* 3. Render rotating, upright sword */
    glPushMatrix();
        /* Position above the anvil */
        glTranslatef(0.0f, 1.75f, -3.0f); 
        
        /* Rotate the sword around Y axis for animation */
        glRotatef(scene->sword_rotation, 0.0f, 1.0f, 0.0f);
        
        /* Stand the sword upright! */
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        
        glScalef(0.03f, 0.03f, 0.03f);

        glBindTexture(GL_TEXTURE_2D, scene->sword_texture);
        draw_model(&(scene->sword));
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);

    /* Draw help overlay if enabled */
    if (scene->show_help) {
        draw_help(scene->help_texture);
    }
}