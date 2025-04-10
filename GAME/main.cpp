#include <iostream>
#include "background.h"
#include "player.h"
#include "SoundManager.h"
#include "textrender.h"
#include "enemy.h"
#include <SDL.h>
#include <sstream>
#include <cmath>
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
bool stop = false;
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

string formatCountdownTime(float remainingSeconds) {
    remainingSeconds = max(0.0f, remainingSeconds);

    int totalSecondsInt = static_cast<int>(remainingSeconds);

    int minutes = totalSecondsInt / 60;
    int seconds = totalSecondsInt % 60;

    stringstream ss;
    ss << (minutes < 10 ? "0" : "") << minutes << ":" << (seconds < 10 ? "0" : "") << seconds;

    return ss.str();
}

void quitSDL() {
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
}
Player P1(4 * 32 - 16,16);
SoundManager soundBackground;
Enemy E1(window_width - 16 - 64, 16,0);
Enemy E2(4 * 32 - 16, window_height - 16 - 64,1);
Enemy E3(window_width - 16 - 64, window_height - 16 - 64,2);
Uint32 lastTime = SDL_GetTicks();
Uint32 currentTime;
float deltaTime = 0.0f;
void reset() {
    P1.reset(4 * 32 - 16,16);
    E1.reset(window_width - 16 - 64, 16);
    E2.reset(4 * 32 - 16, window_height - 16 - 64);
    E3.reset(window_width - 16 - 64, window_height - 16 - 64);
    stt_map = 0;
    enemy_count = 0;
    background.reset();
}
void up_P1() {
    P1.Up_All(renderer,background,deltaTime,soundBackground);
    pair<int,int> pos_P1 = P1.location();
    background.update_character(pos_P1.first,pos_P1.second,1);
}
void up_E1() {
    E1.Up_All(renderer,background,deltaTime,soundBackground);
}
void up_E2() {
    E2.Up_All(renderer,background,deltaTime,soundBackground);
}
void up_E3() {
    E3.Up_All(renderer,background,deltaTime,soundBackground);
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
            if(item == 3000000) {
                E1.update_power();
            }
            if(item == 4000000) {
                E1.up_cnt_bom();
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
            if(item == 3000000) {
                E2.update_power();
            }
            if(item == 4000000) {
                E2.up_cnt_bom();
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
            if(item == 3000000) {
                E3.update_power();
            }
            if(item == 4000000) {
                E3.up_cnt_bom();
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
            }
            if(type == 2000000){
                P1.up_alive();
            }
            if(type == 3000000){
                P1.up_power_bom();
            }
            if(type == 4000000){
                P1.up_cnt_bom();
            }
        }
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
            soundBackground.playSoundEffect("die",0);
        }
        if(background.is_death(pos_E2.first,pos_E2.second) && E2.alive()) {
            E2.down_alive();
            soundBackground.playSoundEffect("die",0);
        }
        if(background.is_death(pos_E3.first,pos_E3.second) && E3.alive()) {
            E3.down_alive();
            soundBackground.playSoundEffect("die",0);
        }
}
void Gameloop() {
    int run = true;
//    SDL_Event e;
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        P1.handleInput(stop);
        if(stop) return;
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
}
bool isLose = false;
bool isWin = false;

int main(int argc, char* argv[])
{
    Init();
    TextRenderer* textrender = new TextRenderer(14);
    int run = 0;
    int cnt = 0;
    const float GAME_DURATION_SECONDS = 120.0f; // Ví dụ: 3 phút
    Uint32 startTime = SDL_GetTicks(); // Thời điểm bắt đầu trò chơi
    float remainingTime = GAME_DURATION_SECONDS; // Thời gian còn lại ban đầu
    bool timeIsUp = false;

    bool game_start = true;

    soundBackground.loadSoundEffect(string("background"),"background.wav");
    soundBackground.loadSoundEffect(string("laser"),"laser.wav");
    soundBackground.loadSoundEffect(string("bom"),"Bom.wav");
    soundBackground.loadSoundEffect(string("lose"),"lose.wav");
    soundBackground.loadSoundEffect(string("win"),"win.wav");
    soundBackground.loadSoundEffect(string("die"),"die.wav");
    soundBackground.playSoundEffect("background",-1);
    while(!run) {
        if(cnt != 5)SDL_RenderClear(renderer), listen_Input();
        SDL_Delay(20);
        if(cnt == 0) {
            background.huong_dan(renderer);
            if(key_space) cnt ++,key_space = false;
        }
        if(cnt == 1) {
            background.print_nen(renderer);
            if(key_space) cnt ++,key_space = false;
        }
        if(cnt == 2) {
            background.print_choosen_map(renderer);
            if(key_1) stt_map = 1;
            if(key_2) stt_map = 2;
            if(key_3) stt_map = 3;
            if(key_esc) cnt --;
            if(key_1 + key_2 + key_3 > 0) cnt ++;
            key_1 = key_2 = key_3 = false;
        }
        if(cnt == 3) {
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
            key_1 = key_2 = key_3 = false;

        }
        if(cnt == 4) {
            background.print_choosen_level(renderer);
            if(key_1 + key_2 + key_3 > 0) cnt ++;
            if(key_2) {
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
            if(key_3) {
                if(enemy_count >= 1) {
                    E1.up_alive();
                    E1.update_power();
                }
                if(enemy_count >= 2) {
                    E2.up_alive();
                    E2.update_power();
                }
                if(enemy_count >= 3) {
                    E3.up_alive();
                    E3.update_power();
                }
            }
            if(key_esc) cnt --;
            key_1 = key_2 = key_3 = false;
        }
        if(cnt == 5) {
            if(game_start) {
                startTime = SDL_GetTicks();
                game_start = false;
                background.typemap(stt_map - 1);
                string type_map = to_string(stt_map) + ".csv";
                background.loadMap(type_map.c_str());
                P1.render_Player(renderer);
                E1.render_Player(renderer);
                E2.render_Player(renderer);
                E3.render_Player(renderer);
            }
                cnt_enemy = 0;
                SDL_Color white = {255, 255, 255, 255};
                SDL_Color black = {0, 0, 0, 255};
            Gameloop();
            if(stop) continue;
            string health_P1 = to_string(P1.alive());
            textrender->renderText(renderer,health_P1,black,72,22);
            string count_bom_P1 =  to_string(P1.Get_count_bom());
            textrender->renderText(renderer,count_bom_P1,black,72,42);
            string power_bom_P1 = to_string(P1.Power_bom());
            textrender->renderText(renderer,power_bom_P1,black,72,62);
            string point_P1 = to_string(P1.Get_point());
            textrender->renderText(renderer,point_P1,black,18,85);

            string health_E1 = to_string(E1.alive());
            textrender->renderText(renderer,health_E1,black,72,134);
            string count_bom_E1 =  to_string(E1.Get_count_bom());
            textrender->renderText(renderer,count_bom_E1,black,72,154);
            string power_bom_E1 = to_string(E1.Power_bom());
            textrender->renderText(renderer,power_bom_E1,black,72,174);
            string point_E1 = to_string(E1.Get_point());
            textrender->renderText(renderer,point_E1,black,18,197);


            string health_E2 = to_string(E2.alive());
            textrender->renderText(renderer,health_E2,black,72,252);
            string count_bom_E2 =  to_string(E2.Get_count_bom());
            textrender->renderText(renderer,count_bom_E2,black,72,272);
            string power_bom_E2 = to_string(E2.Power_bom());
            textrender->renderText(renderer,power_bom_E2,black,72,292);
            string point_E2 = to_string(E2.Get_point());
            textrender->renderText(renderer,point_E2,black,18,313);

            string health_E3 = to_string(E3.alive());
            textrender->renderText(renderer,health_E3,black,72,365);
            string count_bom_E3 =  to_string(E3.Get_count_bom());
            textrender->renderText(renderer,count_bom_E3,black,72,385);
            string power_bom_E3 = to_string(E3.Power_bom());
            textrender->renderText(renderer,power_bom_E3,black,72,405);
            string point_E3 = to_string(E3.Get_point());
            textrender->renderText(renderer,point_E3,black,18,427);

            if(cnt_enemy == 0) {
                isWin = true;
                soundBackground.playSoundEffect("win",0);
                cnt ++;
                continue;
            }
            if (!timeIsUp) {
                Uint32 currentTime = SDL_GetTicks();
                float elapsedTime = (currentTime - startTime) / 1000.0f; // Thời gian đã trôi qua (giây)
                remainingTime = GAME_DURATION_SECONDS - elapsedTime;   // Tính thời gian còn lại

                if (remainingTime <= 0.0f) {
                    remainingTime = 0.0f; // Đảm bảo không âm
                    timeIsUp = true;
                    std::cout << "Hết giờ!" << std::endl;
                }
            }
            string formattedTime = formatCountdownTime(remainingTime);
            SDL_Color timeColor = {255, 255, 255, 255}; // Trắng
            if (remainingTime < 10.0f && !timeIsUp) { // Dưới 10 giây
                 timeColor = {255, 0, 0, 255}; // Đỏ
            } else if (timeIsUp) {
                 timeColor = {255, 0, 0, 255}; // Vẫn giữ màu đỏ khi hết giờ
            }
            textrender->renderText(renderer,formattedTime,timeColor,18,450);
            SDL_RenderPresent(renderer);
            if(timeIsUp) {
                timeIsUp = false;
                cnt++;
                if(P1.Get_point() < max(max(E1.Get_point(),E2.Get_point()),E3.Get_point())) isLose = true,soundBackground.playSoundEffect("lose",0);
                else isWin = true,soundBackground.playSoundEffect("win",0);
                continue;
            }
            if(!P1.alive()) {
                soundBackground.playSoundEffect("die",0);
                cnt ++;
                soundBackground.playSoundEffect("lose",0);
                isLose = true;
                continue;
            }
        }
        if(cnt == 6) {
            if(isLose == true) {
                background.print_Lose(renderer);
            }
            if(isWin == true) {
                background.print_Win(renderer);
            }
            if(key_space == true) {
                cnt = 1,key_space = false;
                isLose = isWin = false;
                remainingTime = GAME_DURATION_SECONDS;
                game_start = true;
                reset();
            }
            else if(key_esc == true) {
                key_esc = false;break;
            }
        }
        if(cnt != 5) {
            SDL_Delay(100);
            SDL_RenderPresent(renderer);
        }
    }
    soundBackground.~SoundManager();
    background.del_all();
    P1.del_all();
    E1.del_all();
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
