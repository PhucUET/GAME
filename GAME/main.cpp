#include <iostream>
#include "background.h"
#include "player.h"
#include "textrender.h"
#include "enemy.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <SDL_image.h>
using namespace std;
const int window_width = 640;
const int window_height = 480;
const int size_texture = 32;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;


bool key_space = false;
bool key_1 = false;
bool key_2 = false;
bool key_3 = false;
bool key_esc = false;

int stt_map = 0;
int enemy_count = 0;


Background background(0,size_texture,window_width,window_height);

void Init() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
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
    TTF_Quit();
}
Player P1(4 * 32 - 16,16);

Enemy E1(window_width - 16 - 64, 16);
Enemy E2(4 * 32 - 16, window_height - 16 - 64);
Enemy E3(window_width - 16 - 64, window_height - 16 - 64);
Uint32 lastTime = SDL_GetTicks();
Uint32 currentTime;
float deltaTime = 0.0f;
void up_P1() {
    P1.Up_All(renderer,background,deltaTime);
}
void up_E1() {
    E1.Up_All(renderer,background,deltaTime);
}
void up_E2() {
    E2.Up_All(renderer,background,deltaTime);
}
void up_E3() {
    E3.Up_All(renderer,background,deltaTime);
}
int cnt_enemy = 0;
void update_pos() {
    if(E1.alive()) {
        cnt_enemy ++;
        pair<int,int> pos_E1 = E1.location();
        background.update_character(pos_E1.first,pos_E1.second,0);
        int item = background.check_item(pos_E1.first,pos_E1.second);
        if(item) {
            if(item == 1000000) {
                E1.change_Gun();
            }
            if(item == 2000000) {
                E1.up_alive();
            }
        }
        E1.nextMove(renderer,background);
        pos_E1 = E1.location();
        background.update_character(pos_E1.first,pos_E1.second,1);
    } else {
         pair<int,int> pos_E1 = E1.location();
        background.update_character(pos_E1.first,pos_E1.second,0);
    }
    if(E2.alive()) {
        cnt_enemy ++;
        pair<int,int> pos_E2 = E2.location();
        background.update_character(pos_E2.first,pos_E2.second,0);
        int item = background.check_item(pos_E2.first,pos_E2.second);
        if(item) {
            if(item == 1000000) {
                E2.change_Gun();
            }
            if(item == 2000000) {
                E2.up_alive();
            }
        }
        E2.nextMove(renderer,background);
        pos_E2 = E2.location();
        background.update_character(pos_E2.first,pos_E2.second,1);
    } else {
         pair<int,int> pos_E2 = E2.location();
        background.update_character(pos_E2.first,pos_E2.second,0);
    }
    if(E3.alive()) {
        cnt_enemy ++;
        pair<int,int> pos_E3 = E3.location();
        background.update_character(pos_E3.first,pos_E3.second,0);
        int item = background.check_item(pos_E3.first,pos_E3.second);
        if(item) {
            if(item == 1000000) {
                E3.change_Gun();
            }
            if(item == 2000000) {
                E3.up_alive();
            }
        }
        E3.nextMove(renderer,background);
        pos_E3 = E3.location();
        background.update_character(pos_E3.first,pos_E3.second,1);
    } else {
         pair<int,int> pos_E3 = E3.location();
        background.update_character(pos_E3.first,pos_E3.second,0);
    }
    if(P1.alive()) {
        pair<int,int> pos_P1 = P1.location();
        background.update_character(pos_P1.first,pos_P1.second,0);
        pos_P1 = P1.location();
        if(int type = background.check_item(pos_P1.first,pos_P1.second)) {
            if(type == 1000000) {
                P1.change_gun();
            } else {
                P1.up_alive();
            }
        }
        background.update_character(pos_P1.first,pos_P1.second,1);
    }
    if(E2.alive()) {
        pair<int,int> pos_E2 = E2.location();
        background.update_character(pos_E2.first,pos_E2.second,0);
        E2.nextMove(renderer,background);
    }
    if(E3.alive()) {
        pair<int,int> pos_E3 = E3.location();
        background.update_character(pos_E3.first,pos_E3.second,0);
        E3.nextMove(renderer,background);
    }
}

void up_death() {
        pair<int,int> pos_P1 = P1.location();
        pair<int,int> pos_E1 = E1.location();
        pair<int,int> pos_E2 = E2.location();
        pair<int,int> pos_E3 = E3.location();
        if(background.is_death(pos_P1.first,pos_P1.second)) {
            cout <<"thang nhan vat chet toi \n";
           P1.down_alive();
        }
        if(background.is_death(pos_E1.first,pos_E1.second) && E1.alive()) {
            E1.down_alive();
        }
        if(background.is_death(pos_E2.first,pos_E2.second) && E2.alive()) {
            E2.down_alive();
        }
        if(background.is_death(pos_E3.first,pos_E3.second) && E3.alive()) {
            E3.down_alive();
        }
}
void Gameloop() {
    int run = true;
//    SDL_Event e;
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        P1.handleInput();
        update_pos();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        background.BackgroundD(renderer);
         up_P1();
        if(E1.alive()) {
            up_E1();
        }
        if(E2.alive()) {
            up_E2();
        }
        if(E3.alive()) {
            up_E3();
        }
        up_death();
        background.reset_death();
//    cout <<"thua cuoc \n";
}


void listen_Input() {
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_KEYDOWN) {
            cout <<"ngu\n";
            switch (e.key.keysym.sym) {
            case SDLK_1:
                key_1 = true;
                break;
            case SDLK_2:
                key_2 = true;
                break;
            case SDLK_3:
                key_3 = true;
                break;
            case SDLK_ESCAPE:
                key_esc = true;
                break;
            case SDLK_SPACE:
                key_space = true;
                break;
            }
        } else if(e.type == SDL_KEYUP) {
            switch (e.key.keysym.sym) {
            case SDLK_1:
                key_1 = false;
                break;
            case SDLK_2:
                key_2 = false;
                break;
            case SDLK_3:
                key_3 = false;
                break;
            case SDLK_ESCAPE:
                key_esc = false;
                break;
            case SDLK_SPACE:
                key_space = false;
                break;
            }
        }
    }
    cout << key_1 <<" "<<key_2 <<" "<<key_3 <<" " << key_space <<"\n";
}

int main(int argc, char* argv[])
{
    Init();
    TextRenderer* textrender = new TextRenderer(14);
    int run = 0;
    int cnt = 0;
    bool game_start = true;
    while(!run) {
        if(cnt < 4)SDL_RenderClear(renderer), listen_Input();
        SDL_Delay(30);
        if(cnt == 0) {
            background.print_nen(renderer);
            if(key_space) cnt ++;
        }
        if(cnt == 1) {
            background.print_choosen_map(renderer);
            if(key_1) stt_map = 1;
            if(key_2) stt_map = 2;
            if(key_3) stt_map = 3;
            if(key_esc) cnt --;
            if(key_1 + key_2 + key_3 > 0) cnt ++;
            key_1 = key_2 = key_3 = false;
        }
        if(cnt == 2) {
            background.print_choosen_enemy(renderer);
            if(key_1 + key_2 + key_3 > 0) cnt ++;
            if(key_esc) cnt --;
            if(key_1) enemy_count = 1;
            if(key_2) enemy_count = 2;
            if(key_3) enemy_count = 3;
            if(enemy_count >= 1) {
                E1.up_alive();
            }
            if(enemy_count >= 2) {
                E2.up_alive();
            }
            if(enemy_count >= 3) {
                E3.up_alive();
            }
        }
        if(cnt == 3) {
            background.print_choosen_level(renderer);
            if(key_1 + key_2 + key_3 > 0) cnt ++;
            key_1 = key_2 = key_3 = false;
            if(key_esc) cnt --;
        }
        if(cnt == 4) {
            if(game_start) {
                game_start = false;
                background.typemap(stt_map - 1);
                string type_map = to_string(stt_map) + ".csv";
                background.loadMap("1.csv");
                P1.render_Player(renderer);
                E1.render_Player(renderer);
                E2.render_Player(renderer);
                E3.render_Player(renderer);
                E1.change_Gun();
            }
            cnt_enemy = 0;
                SDL_Color white = {255, 255, 255, 255};
                SDL_Color black = {0, 0, 0, 255};
//            cout <<"ngusi dan don \n";

            Gameloop();

            string health = to_string(P1.alive());
            textrender->renderText(renderer,health,black,72,22);
//            string count_bom =  to_string(P1.Get_count_bom());
//            textrender->renderText(renderer,count_bom,black,72,44);






            SDL_RenderPresent(renderer);
            if(cnt_enemy == 0) {
                cout <<"you win\n";
                break;
            }
        }
        if(cnt < 4) {
            SDL_Delay(100);
            SDL_RenderPresent(renderer);
        }
//         while(SDL_PollEvent(&e))
//            if (e.type == SDL_QUIT) run = false;
    }

//    P1.render_Player(renderer);
//    background.typemap(0);
//    background.loadMap("1.csv");

//    SDL_RenderPresent(renderer);
////    E1.down_alive();
//    P1.update_power();
//    Gameloop();
    quitSDL();
    return 0;
}



//178 292          390 292
//178 386          390 386

//166 192          396 192    level
//166 248          396 248

//166 278          396 278
//166 334          396 334

//166 368
//166 420

//map
// 80 294     158 294
