#include <iostream>
#include "background.h"
#include "player.h"
#include <SDL.h>
#include <SDL_image.h>
using namespace std;
const int window_width = 576;
const int window_height = 480;
const int size_texture = 32;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

Background background(0,size_texture,window_width,window_height);

void Init() {
    window = SDL_CreateWindow("GAME_LOP",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,window_width,window_height,SDL_WINDOW_SHOWN);
    if(!window) {
        cerr << "Khongthekhoitaowindow" << SDL_GetError() <<"\n";
        exit(EXIT_FAILURE);
    }
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer) {
        cerr <<"Khongthekhoitao Render" << SDL_GetError() <<"\n";
         exit(EXIT_FAILURE);
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
}

void quitSDL() {
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
Player P1(3 * 32 - 16,-16);

void Gameloop() {
    int run = true;
    SDL_Event e;
    while(run) {
        cerr << "hehe\n";
        P1.handleInput();
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                run = false;
            }
        }
//        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//        SDL_RenderClear(renderer);
        background.BackgroundD(renderer);
        P1.Up_All(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }
}

int main(int argc, char* argv[])
{
    Init();
    P1.render_Player(renderer);
    background.typemap(0);
    Gameloop();

    quitSDL();
    return 0;
}
