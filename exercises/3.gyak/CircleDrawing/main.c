#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define MAX_CIRCLE_COUNT 50
#define PALETTE_HEIGHT 40
#define COLOR_BUTTON_SIZE 40
#define SEGMENTS 36
#define HOVER_MARK_SIZE 10

typedef struct {
    Uint8 r;
    Uint8 g;
    Uint8 b;
} Color;

typedef struct {
    SDL_Point center;
    int radius;
    Color color;
    bool is_moving;
} Circle;

const Color PALETTE[] = {
    {255,0,0}, {0,255,0}, {0,0,255}, 
    {255,255,0}, {0,255,255}, {255,255,255}
};
const int PALETTE_SIZE = sizeof(PALETTE)/sizeof(PALETTE[0]);

Circle circles[MAX_CIRCLE_COUNT];
int circle_count = 0;
Color current_color = {255,255,255};
SDL_Point drag_offset = {0,0};

void draw_circle(SDL_Renderer* renderer, Circle circle) {
    SDL_Point points[SEGMENTS+1];
    
    for(int i = 0; i <= SEGMENTS; i++) {
        double angle = 2 * M_PI * i / SEGMENTS;
        points[i].x = circle.center.x + circle.radius * cos(angle);
        points[i].y = circle.center.y + circle.radius * sin(angle);
    }
    
    SDL_SetRenderDrawColor(renderer, 
        circle.color.r, 
        circle.color.g, 
        circle.color.b, 
        255
    );
    
    for(int i = 0; i < SEGMENTS; i++) {
        SDL_RenderDrawLine(renderer, 
            points[i].x, points[i].y, 
            points[i+1].x, points[i+1].y
        );
    }
}

void draw_hover_mark(SDL_Renderer* renderer, SDL_Point center) {
    SDL_RenderDrawLine(renderer, 
        center.x - HOVER_MARK_SIZE, center.y,
        center.x + HOVER_MARK_SIZE, center.y
    );
    SDL_RenderDrawLine(renderer, 
        center.x, center.y - HOVER_MARK_SIZE,
        center.x, center.y + HOVER_MARK_SIZE
    );
}

bool point_in_circle(SDL_Point p, Circle c) {
    int dx = p.x - c.center.x;
    int dy = p.y - c.center.y;
    return (dx*dx + dy*dy) <= (c.radius*c.radius);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window* window = SDL_CreateWindow(
        "Kör Rajzoló", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        800, 600, 
        SDL_WINDOW_SHOWN
    );
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    bool quit = false;
    bool drawing_circle = false;
    SDL_Point temp_center;
    Circle* dragged_circle = NULL;

    while(!quit) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                
                case SDL_MOUSEBUTTONDOWN: {
                    SDL_Point mouse = {event.button.x, event.button.y};
                    
                    // Színválasztás
                    if(mouse.y < PALETTE_HEIGHT) {
                        int index = mouse.x / COLOR_BUTTON_SIZE;
                        if(index < PALETTE_SIZE) current_color = PALETTE[index];
                    }
                    // Kör mozgatás
                    else {
                        for(int i = circle_count-1; i >= 0; i--) {
                            if(point_in_circle(mouse, circles[i])) {
                                circles[i].is_moving = true;
                                drag_offset.x = mouse.x - circles[i].center.x;
                                drag_offset.y = mouse.y - circles[i].center.y;
                                dragged_circle = &circles[i];
                                break;
                            }
                        }
                        // Új kör kezdése
                        if(!dragged_circle && circle_count < MAX_CIRCLE_COUNT) {
                            if(!drawing_circle) {
                                temp_center = mouse;
                                drawing_circle = true;
                            } else {
                                int dx = mouse.x - temp_center.x;
                                int dy = mouse.y - temp_center.y;
                                circles[circle_count] = (Circle){
                                    temp_center,
                                    (int)sqrt(dx*dx + dy*dy),
                                    current_color,
                                    false
                                };
                                circle_count++;
                                drawing_circle = false;
                            }
                        }
                    }
                    break;
                }
                
                case SDL_MOUSEBUTTONUP:
                    if(dragged_circle) {
                        dragged_circle->is_moving = false;
                        dragged_circle = NULL;
                    }
                    break;
                
                case SDL_MOUSEMOTION:
                    if(dragged_circle) {
                        dragged_circle->center.x = event.motion.x - drag_offset.x;
                        dragged_circle->center.y = event.motion.y - drag_offset.y;
                    }
                    break;
            }
        }

        // Háttér
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Paletta
        for(int i = 0; i < PALETTE_SIZE; i++) {
            SDL_Rect btn = {i*COLOR_BUTTON_SIZE, 0, COLOR_BUTTON_SIZE, PALETTE_HEIGHT};
            SDL_SetRenderDrawColor(renderer, PALETTE[i].r, PALETTE[i].g, PALETTE[i].b, 255);
            SDL_RenderFillRect(renderer, &btn);
        }

        // Körök kirajzolása
        for(int i = 0; i < circle_count; i++) {
            draw_circle(renderer, circles[i]);
        }

        // Aktuális kör előnézete
        if(drawing_circle) {
            SDL_Point mouse;
            SDL_GetMouseState(&mouse.x, &mouse.y);
            int dx = mouse.x - temp_center.x;
            int dy = mouse.y - temp_center.y;
            Circle preview = {
                temp_center,
                (int)sqrt(dx*dx + dy*dy),
                current_color,
                false
            };
            draw_circle(renderer, preview);
        }

        // Hover jelzés
        SDL_Point mouse;
        SDL_GetMouseState(&mouse.x, &mouse.y);
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        for(int i = 0; i < circle_count; i++) {
            if(point_in_circle(mouse, circles[i])) {
                draw_hover_mark(renderer, circles[i].center);
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}