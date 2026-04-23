#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_LINE_COUNT 100
#define PALETTE_HEIGHT 40
#define COLOR_BUTTON_SIZE 80

typedef struct {
    Uint8 r;
    Uint8 g;
    Uint8 b;
} Color;

typedef struct {
    SDL_Point start;
    SDL_Point end;
    Color color;
} Line;

// Előre definiált színek
const Color PALETTE[] = {
    {255, 0, 0},    // Piros
    {0, 255, 0},    // Zöld
    {0, 0, 255},    // Kék
    {255, 255, 0},  // Sárga
    {255, 0, 255},  // Magenta
    {0, 255, 255},  // Cián
    {255, 255, 255} // Fehér
};
const int PALETTE_SIZE = sizeof(PALETTE) / sizeof(PALETTE[0]);

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window* window = SDL_CreateWindow(
        "Szakasz Rajzoló", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        800, 600, 
        SDL_WINDOW_SHOWN
    );
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    Line lines[MAX_LINE_COUNT];
    int line_count = 0;
    SDL_Point temp_start;
    bool is_drawing = false;
    Color current_color = {255, 255, 255}; // Alapértelmezett fehér

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
                        int x = event.button.x;
                        int y = event.button.y;

                        // Paletta kezelése
                        if(y < PALETTE_HEIGHT) {
                            int color_index = x / COLOR_BUTTON_SIZE;
                            if(color_index < PALETTE_SIZE) {
                                current_color = PALETTE[color_index];
                            }
                        }
                        // Szakasz rajzolás kezelése
                        else {
                            if(!is_drawing) {
                                temp_start.x = x;
                                temp_start.y = y;
                                is_drawing = true;
                            } else {
                                if(line_count < MAX_LINE_COUNT) {
                                    lines[line_count] = (Line){
                                        temp_start,
                                        {x, y},
                                        current_color
                                    };
                                    line_count++;
                                }
                                is_drawing = false;
                            }
                        }
                    }
                    break;
            }
        }

        // Háttér törlése
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Paletta kirajzolása
        for(int i = 0; i < PALETTE_SIZE; i++) {
            SDL_Rect color_rect = {
                i * COLOR_BUTTON_SIZE,
                0,
                COLOR_BUTTON_SIZE,
                PALETTE_HEIGHT
            };
            SDL_SetRenderDrawColor(renderer, 
                PALETTE[i].r, 
                PALETTE[i].g, 
                PALETTE[i].b, 
                255
            );
            SDL_RenderFillRect(renderer, &color_rect);
        }

        // Szakaszok kirajzolása
        for(int i = 0; i < line_count; i++) {
            SDL_SetRenderDrawColor(renderer, 
                lines[i].color.r,
                lines[i].color.g,
                lines[i].color.b,
                255
            );
            SDL_RenderDrawLine(renderer, 
                lines[i].start.x, 
                lines[i].start.y, 
                lines[i].end.x, 
                lines[i].end.y
            );
        }

        // Aktuális szakasz előnézete
        if(is_drawing) {
            int mouse_x, mouse_y;
            SDL_GetMouseState(&mouse_x, &mouse_y);
            SDL_SetRenderDrawColor(renderer, 
                current_color.r,
                current_color.g,
                current_color.b,
                255
            );
            SDL_RenderDrawLine(renderer, 
                temp_start.x, 
                temp_start.y, 
                mouse_x, 
                mouse_y
            );
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}