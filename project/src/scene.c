#include <stdlib.h>
#include <time.h>
#include "scene.h"
#include <GL/gl.h>
#include <stdio.h>
#include <obj/load.h>
#include <obj/draw.h>

/* Helper function to add a bounding box to the scene */
void add_box(Scene* scene, float min_x, float max_x, float min_z, float max_z) {
    if (scene->box_count < 15) {
        scene->boxes[scene->box_count].min_x = min_x;
        scene->boxes[scene->box_count].max_x = max_x;
        scene->boxes[scene->box_count].min_z = min_z;
        scene->boxes[scene->box_count].max_z = max_z;
        scene->box_count++;
    }
}

/* Helper function to create a shadow projection matrix for the Y=0 plane */
void calculate_shadow_matrix(float m[16], float light_pos[4]) {
    /* The floor is at Y=0, so the plane equation is: 0x + 1y + 0z + 0 = 0 */
    float dot = light_pos[1]; /* Plane normal (0,1,0) dot light_pos */

    for (int i = 0; i < 16; i++) m[i] = 0.0f;

    m[0] = dot;
    m[5] = 0.0f; 
    m[10] = dot;
    m[15] = dot;

    m[4] = -light_pos[0];
    m[6] = -light_pos[2];
    m[7] = -1.0f;
}

void init_scene(Scene* scene) {
    srand(time(NULL));
    glEnable(GL_TEXTURE_2D);

    /* Initialize light, fog intensity to default */
    scene->light_intensity = 1.0f;
    scene->fog_density = 0.08f;
    scene->selected_object_id = 0; /* Default: no object selected */

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

    scene->door_texture = load_texture("assets/textures/door.png");
    if (!load_model(&(scene->door), "assets/models/door.obj")) {
        printf("Error: Failed to load door model!\n");
    }

    scene->door_frame_texture = load_texture("assets/textures/door_frame.png");
    if (!load_model(&(scene->door_frame), "assets/models/door_frame.obj")) {
        printf("Error: Failed to load door knob model!\n");
    }
    
    scene->door_knob_texture = load_texture("assets/textures/door_knob.png");
    if (!load_model(&(scene->door_knob), "assets/models/door_knob.obj")) {
        printf("Error: Failed to load door knob model!\n");
    }

    scene->window_texture = load_texture("assets/textures/window.png");
    if (!load_model(&(scene->window), "assets/models/window.obj")) {
        printf("Error: Failed to load window model!\n");
    }

    scene->window_screen_texture = load_texture("assets/textures/window_screen.png");
    if (!load_model(&(scene->window_screen), "assets/models/window_screen.obj")) {
        printf("Error: Failed to load window model!\n");
    }

    scene->sword_texture = load_texture("assets/textures/sword.png");
    if (scene->sword_texture == 0) {
        printf("Error: Failed to load sword texture!\n");
    }

    if (!load_model(&(scene->sword), "assets/models/sword.obj")) {
        printf("Error: Failed to load sword model!\n");
    }

    scene->forge_texture = load_texture("assets/textures/forge.png");
    if (!load_model(&(scene->forge), "assets/models/forge.obj")) {
        printf("Error: Failed to load forge model!\n");
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

    /* 3. Box for the forge */
    add_box(scene, -6.0f, -2.0f, -10.0f, -7.8f);

    /* 4. Box for the door */
    add_box(scene, -10.5f, -9.5f, -1.3f, 1.3f);

    /* 5. Box for Window 1 */
    add_box(scene, 9.5f, 10.0f, -2.5f, 2.5f);

    /* 6. Box for Window 2 */
    add_box(scene, -2.5f, 2.5f, 9.5f, 10.0f);

    /* Initialize help overlay */
    scene->show_help = 0;
    scene->help_texture = load_texture("assets/textures/help.png");

    /* Initialize particles (Zero out everything to prevent memory garbage crashes!) */
    for (int p = 0; p < 100; p++) {
        scene->particles[p].x = 0.0f;
        scene->particles[p].y = 0.0f;
        scene->particles[p].z = 0.0f;
        scene->particles[p].vx = 0.0f;
        scene->particles[p].vy = 0.0f;
        scene->particles[p].vz = 0.0f;
        scene->particles[p].life = 0.0f; /* Start dead to respawn immediately */
        scene->particles[p].fade = 1.0f; /* Safe fade value */
    }
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

    /* Update particles (Move and respawn sparks) */
    for (int p = 0; p < 100; p++) {
        /* Move based on velocity and time step */
        scene->particles[p].x += scene->particles[p].vx * (float)time_step;
        scene->particles[p].y += scene->particles[p].vy * (float)time_step;
        scene->particles[p].z += scene->particles[p].vz * (float)time_step;
        
        /* Decrease lifespan */
        scene->particles[p].life -= scene->particles[p].fade * (float)time_step;

        /* Respawn if dead */
        if (scene->particles[p].life <= 0.0f) {
            scene->particles[p].life = 1.0f;
            scene->particles[p].fade = (float)(rand() % 100) / 100.0f + 0.4f;
            
            /* Spawn position */
            scene->particles[p].x = -4.0f + ((float)(rand() % 100) / 100.0f - 0.5f) * 0.6f;
            scene->particles[p].y = 0.15f; /* Fire height */
            scene->particles[p].z = -9.4f + ((float)(rand() % 100) / 100.0f - 0.5f) * 0.6f;
            
            scene->particles[p].vx = ((float)(rand() % 100) / 100.0f - 0.5f) * 1.0f;
            scene->particles[p].vy = ((float)(rand() % 100) / 100.0f) * 2.0f + 0.5f;
            scene->particles[p].vz = ((float)(rand() % 100) / 100.0f - 0.5f) * 1.0f;
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

    /* Basic light color
    GLfloat ambient_light[]  = { 0.15f, 0.15f, 0.15f, 1.0f }; 
    GLfloat diffuse_light[]  = { 0.8f * i, 0.8f * i, 0.8f * i, 1.0f };
    GLfloat specular_light[] = { 1.0f * i, 1.0f * i, 1.0f * i, 1.0f };
    */

    /* Orange tint */
    GLfloat ambient_light[]  = { 0.2f, 0.1f, 0.05f, 1.0f }; 
    GLfloat diffuse_light[]  = { 1.0f * i, 0.5f * i, 0.2f * i, 1.0f };
    GLfloat specular_light[] = { 1.0f * i, 0.8f * i, 0.5f * i, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);

    GLfloat light_position[] = { -4.0f, 6.0f, -7.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glColor3f(1.0f, 1.0f, 1.0f);

    /* FOG */
    glEnable(GL_FOG);
    
    GLfloat fog_color[] = { 0.15f, 0.15f, 0.15f, 1.0f };
    glFogfv(GL_FOG_COLOR, fog_color);
    
    glFogi(GL_FOG_MODE, GL_EXP);
    
    /* Density */
    glFogf(GL_FOG_DENSITY, scene->fog_density);
    
    glHint(GL_FOG_HINT, GL_NICEST);

/* --- RENDERING SHADOWS --- */
    float shadow_mat[16];
    calculate_shadow_matrix(shadow_mat, light_position);

    glDisable(GL_LIGHTING);   /* Shadow should be a solid dark color */
    glDisable(GL_TEXTURE_2D); /* No texturing for shadows */
    glEnable(GL_BLEND);       /* Enable transparency */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    /* Enable polygon offset to prevent Z-fighting with the floor */
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(-1.0f, -1.0f);

    glColor4f(0.0f, 0.0f, 0.0f, 0.4f); /* Semi-transparent black color */

    /* 1. Anvil shadow */
    glPushMatrix();
        glMultMatrixf(shadow_mat);
        glTranslatef(0.0f, 0.0f, -3.0f); 
        glScalef(0.4f, 0.4f, 0.4f); 
        draw_model(&(scene->anvil));
    glPopMatrix();

    /* 2. Sword shadow */
    glPushMatrix();
        glMultMatrixf(shadow_mat);
        glTranslatef(0.0f, 1.75f, -3.0f); 
        glRotatef(scene->sword_rotation, 0.0f, 1.0f, 0.0f); 
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f); 
        glScalef(0.03f, 0.03f, 0.03f); 
        draw_model(&(scene->sword));
    glPopMatrix();

    /* 3. Forge shadow */
    glPushMatrix();
        glMultMatrixf(shadow_mat);
        glTranslatef(-4.0f, 0.0f, -9.4f);
        draw_model(&(scene->forge));
    glPopMatrix();

    glDisable(GL_POLYGON_OFFSET_FILL);

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
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

        /* Highlight if selected */
        if (scene->selected_object_id == 1) {
            glColor3f(1.0f, 0.5f, 0.5f); /* Reddish tint */
        } else {
            glColor3f(1.0f, 1.0f, 1.0f);
        }

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
        
        /* Stand the sword upright */
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        
        glScalef(0.03f, 0.03f, 0.03f);

        /* Highlight if selected */
        if (scene->selected_object_id == 2) {
            glColor3f(1.0f, 0.5f, 0.5f); /* Reddish tint */
        } else {
            glColor3f(1.0f, 1.0f, 1.0f);
        }

        glBindTexture(GL_TEXTURE_2D, scene->sword_texture);
        draw_model(&(scene->sword));
    glPopMatrix();

    /* 4. Render Forge */
    glPushMatrix();
        glTranslatef(-4.0f, -0.4f, -9.4f);
        if (scene->selected_object_id == 3) glColor3f(1.0f, 0.5f, 0.5f);
        else glColor3f(1.0f, 1.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, scene->forge_texture);
        draw_model(&(scene->forge));
    glPopMatrix();

    /* 5. Render Door */
    glPushMatrix();
        glTranslatef(-9.9f, 1.45f, 0.0f);
        glScalef(0.5f, 0.45f, 0.505f);
        if (scene->selected_object_id == 4) glColor3f(1.0f, 0.5f, 0.5f);
        else glColor3f(1.0f, 1.0f, 1.0f);

        /* 1. Door piece */
        glBindTexture(GL_TEXTURE_2D, scene->door_texture);
        draw_model(&(scene->door));

        /* 2. Door frame */
        glTranslatef(0.001f, 0.001f, -0.001f);
        glBindTexture(GL_TEXTURE_2D, scene->door_frame_texture);
        draw_model(&(scene->door_frame));

        /* 3. Door knob */
        glTranslatef(0.01f, 0.001f, 0.001f);
        glBindTexture(GL_TEXTURE_2D, scene->door_knob_texture);
        draw_model(&(scene->door_knob));
    glPopMatrix();

    /* 6. Render Window 1 */
    glPushMatrix();
        glTranslatef(9.8f, 1.0f, 0.0f);
        glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
        glScalef(0.6f, 0.5f, 0.5f);
        if (scene->selected_object_id == 5) glColor3f(1.0f, 0.5f, 0.5f);
        else glColor3f(1.0f, 1.0f, 1.0f);

        glBindTexture(GL_TEXTURE_2D, scene->window_texture);
        draw_model(&(scene->window));
        glTranslatef(0.0f, 0.0f, 0.01f);
        glBindTexture(GL_TEXTURE_2D, scene->window_screen_texture);
        draw_model(&(scene->window_screen));
    glPopMatrix();

    /* 7. Render Window 2 */
    glPushMatrix();
        glTranslatef(0.0f, 1.0f, 9.8f);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glScalef(0.6f, 0.5f, 0.5f);
        if (scene->selected_object_id == 6) glColor3f(1.0f, 0.5f, 0.5f);
        else glColor3f(1.0f, 1.0f, 1.0f);

        glBindTexture(GL_TEXTURE_2D, scene->window_texture);
        draw_model(&(scene->window));
        glTranslatef(0.0f, 0.0f, 0.01f);
        glBindTexture(GL_TEXTURE_2D, scene->window_screen_texture);
        draw_model(&(scene->window_screen));
    glPopMatrix();

    /* 8. Render particles (Draw sparks) */
    /* Disable lighting and textures so the particles "glow" with their own color */
    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPointSize(5.0f); /* Size of the sparks */

    glBegin(GL_POINTS);
    for (int p = 0; p < 100; p++) {
        if (scene->particles[p].life > 0.0f) {
            /* Color: Starts yellow and turns reddish as it ages (life decreases) */
            glColor3f(1.0f, scene->particles[p].life, 0.0f);
            glVertex3f(scene->particles[p].x, scene->particles[p].y, scene->particles[p].z);
        }
    }
    glEnd();
    
    /* Re-enable lighting for other objects */
    glEnable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, 0);

    /* Draw help overlay if enabled */
    if (scene->show_help) {
        draw_help(scene->help_texture);
    }
}

/* Color Picking Implementation */
int pick_object(Scene* scene, const Camera* camera, int x, int y, int width, int height) {
    (void)width; /* Avoid unused parameter warning */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    set_view(camera);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_TEXTURE_2D);

    /* ID 1: Anvil */
    glPushMatrix();
        glTranslatef(0.0f, -0.4f, -3.0f);
        glScalef(0.4f, 0.4f, 0.4f);
        glColor3ub(1, 0, 0); 
        draw_model(&(scene->anvil));
    glPopMatrix();

    /* ID 2: Sword */
    glPushMatrix();
        glTranslatef(0.0f, 1.75f, -3.0f);
        glRotatef(scene->sword_rotation, 0.0f, 1.0f, 0.0f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glScalef(0.03f, 0.03f, 0.03f);
        glColor3ub(2, 0, 0);
        draw_model(&(scene->sword));
    glPopMatrix();

    /* ID 3: Forge */
    glPushMatrix();
        glTranslatef(-4.0f, -0.4f, -9.4f);
        glColor3ub(3, 0, 0); 
        draw_model(&(scene->forge));
    glPopMatrix();

    /* ID 4: Door */
    glPushMatrix();
        glTranslatef(-9.8f, 1.0f, 0.0f);
        glRotatef(0.0f, 90.0f, 0.0f, 0.0f);
        glColor3ub(4, 0, 0); 
        draw_model(&(scene->door));
    glPopMatrix();

    /* ID 5: Window 1 */
    glPushMatrix();
        glTranslatef(9.8f, 1.0f, 0.0f);
        glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
        glColor3ub(5, 0, 0); 
        draw_model(&(scene->window));
    glPopMatrix();

    /* ID 6: Window 2 */
    glPushMatrix();
        glTranslatef(0.0f, 1.2f, 9.8f);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glScalef(0.6f, 0.6f, 0.6f);
        glColor3ub(6, 0, 0); draw_model(&(scene->window));
    glPopMatrix();

    unsigned char pixel[3];
    glReadPixels(x, height - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_FOG);
    glEnable(GL_LIGHTING);

    return (int)pixel[0];
}