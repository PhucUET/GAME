#include <iostream>
#include "background.h"
#include "player.h"
#include "enemy.h"
#include <SDL.h>
#include <SDL_image.h>
using namespace std;
const int window_width = 640;
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
Player P1(4 * 32 - 16,16);

Enemy E1(640 - 16 - 64, 16);


Uint32 lastTime = SDL_GetTicks();
Uint32 currentTime;
float deltaTime = 0.0f;
void up_P1() {
    P1.Up_All(renderer,background,deltaTime);
}
void up_E1() {
    E1.Up_All(renderer,background,deltaTime);
}
void update_pos() {
    if(E1.alive()) {
        pair<int,int> pos_E1 = E1.location();
        background.update_character(pos_E1.first,pos_E1.second,0);
        E1.nextMove(renderer,background);
        pos_E1 = E1.location();
        background.update_character(pos_E1.first,pos_E1.second,1);
    } else {
         pair<int,int> pos_E1 = E1.location();
        background.update_character(pos_E1.first,pos_E1.second,0);
    }
    if(P1.alive()) {
        pair<int,int> pos_P1 = P1.location();
        background.update_character(pos_P1.first,pos_P1.second,0);
        P1.update(background,renderer);
        pos_P1 = P1.location();
        background.update_character(pos_P1.first,pos_P1.second,1);
    }
//    if(E2.alive()) {
//        pair<int,int> pos_E2 = E2.location();
//        background.update_character(pos_E2.first,pos_E2.second,0);
//        E2.nextMove(renderer,background);
//    }
//    if(E3.alive()) {
//        pair<int,int> pos_E3 = E3.location();
//        background.update_character(pos_E3.first,pos_E3.second,0);
//        E3.nextMove(renderer,background);
//    }
}

void Gameloop() {
    int run = true;
    SDL_Event e;
    while(P1.alive()) {
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        cerr << "hehe\n";
        P1.handleInput();
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                run = false;
            }
        }
        update_pos();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        background.BackgroundD(renderer);
        up_P1();
        if(E1.alive()) {
            up_E1();
        }
        pair<int,int> pos_P1 = P1.location();
        pair<int,int> pos_E1 = E1.location();
        if(background.is_death(pos_P1.first,pos_P1.second)) {
            cout <<"thang nhan vat chet toi \n";
           P1.down_alive();
        }
        if(background.is_death(pos_E1.first,pos_E1.second) && E1.alive()) {
            E1.down_alive();
//            cout << "enemy con song khong vayyyyyy" << E1.alive() <<"\n";
        }
        cout <<"so diem cua ban la" <<" "<<P1.Get_point() <<"\n";
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
        background.reset_death();

    }
    cout <<"thua cuoc \n";
}

int main(int argc, char* argv[])
{
    Init();
    background.typemap(0);
    background.loadMap("1.csv");
    P1.render_Player(renderer);
    E1.render_Player(renderer);
    SDL_RenderPresent(renderer);
    P1.update_power();
    Gameloop();
    quitSDL();
    return 0;
}
