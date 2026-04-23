#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_RECT_COUNT 100
#define PALETTE_HEIGHT 40
#define COLOR_BUTTON_SIZE 40

typedef struct {
    Uint8 r;
    Uint8 g;
    Uint8 b;
} Color;

typedef struct {
    SDL_Rect rect;
    Color color;
} Rectangle;

const Color PALETTE[] = {
    {255, 0, 0}, {0, 255, 0}, {0, 0, 255},
    {255, 255, 0}, {255, 0, 255}, {0, 255, 255}, {255, 255, 255}
};
const int PALETTE_SIZE = sizeof(PALETTE) / sizeof(PALETTE[0]);

SDL_Rect calculate_rect(SDL_Point start, SDL_Point end) {
    return (SDL_Rect){
        .x = start.x < end.x ? start.x : end.x,
        .y = start.y < end.y ? start.y : end.y,
        .w = abs(end.x - start.x),
        .h = abs(end.y - start.y)
    };
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window* window = SDL_CreateWindow(
        "Teglalap Rajzolo", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        800, 600, 
        SDL_WINDOW_SHOWN
    );
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    Rectangle rectangles[MAX_RECT_COUNT];
    int rect_count = 0;
    SDL_Point temp_start;
    bool is_drawing = false;
    Color current_color = PALETTE[0];

    bool quit = false;
    while(!quit) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                
                case SDL_MOUSEBUTTONDOWN:
                    if(event.button.button == SDL_BUTTON_LEFT) {
                        SDL_Point mouse = {event.button.x, event.button.y};
                        
                        if(mouse.y < PALETTE_HEIGHT) {
                            int index = mouse.x / COLOR_BUTTON_SIZE;
                            if(index < PALETTE_SIZE) current_color = PALETTE[index];
                        } else {
                            if(!is_drawing) {
                                temp_start = mouse;
                                is_drawing = true;
                            } else {
                                if(rect_count < MAX_RECT_COUNT) {
                                    rectangles[rect_count].rect = calculate_rect(temp_start, mouse);
                                    rectangles[rect_count].color = current_color;
                                    rect_count++;
                                }
                                is_drawing = false;
                            }
                        }
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

        // Téglalapok
        for(int i = 0; i < rect_count; i++) {
            SDL_SetRenderDrawColor(renderer, 
                rectangles[i].color.r,
                rectangles[i].color.g,
                rectangles[i].color.b,
                255
            );
            SDL_RenderFillRect(renderer, &rectangles[i].rect);
        }

        // Előnézet
        if(is_drawing) {
            SDL_Point current_mouse;
            SDL_GetMouseState(&current_mouse.x, &current_mouse.y);
            SDL_Rect preview = calculate_rect(temp_start, current_mouse);
            
            SDL_SetRenderDrawColor(renderer, 
                current_color.r,
                current_color.g,
                current_color.b,
                100 // Átlátszóság
            );
            SDL_RenderFillRect(renderer, &preview);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}