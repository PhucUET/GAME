#ifndef PLAYER.H
#define PLAYER.H

#include "weapon.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "background.h"
#include <utility>
#include <vector>
class Player{
private:
    SDL_Rect dstRect;
    SDL_Rect srcRect = { 0, 0, 64, 64};

    SDL_Texture* idleTexture = nullptr;
    SDL_Texture* runTexture = nullptr;
    SDL_Texture* currentTexture = nullptr;
    SDL_Renderer* renderer_player = nullptr;

    bool key_w = false;
    bool key_s = false;
    bool key_d = false;
    bool key_a = false;
    bool key_space = false;
    bool key_esc = false;

    int speed = 32;
    int health  = 1;
    int point = 0;

    bool moving = false;
    bool isrunning = false;
    Uint32 lastTime = SDL_GetTicks();

    int cnt_bom = 1;
    vector<Weapon> weaponss;

    int dx[4] = {0,0,-32,32};
    int dy[4] = {32,-32,0,0};

public:
    void del_all() {
        SDL_DestroyTexture(idleTexture);
        SDL_DestroyTexture(runTexture);
        SDL_DestroyTexture(currentTexture);
    }
    int Get_count_bom() {
        return weaponss.size();
    }
    int Get_point() {
        return point;
    }
    Player(int sx,int sy);
    pair<int,int> location() {
        return make_pair(dstRect.x,dstRect.y);
    }
    void up_cnt_bom() {
        Weapon new_weapon;
        weaponss.push_back(new_weapon);
    }
    int idleWidth = 0, idleHeight = 0;
    int runWidth = 0, runHeight = 0;
    int frame_count = 0;
    int frame_count_run = 0; // hiện tại đang ở frame bao nhiêu của run
    int currentAnimation = 0; // tổng số frame trong animation hiện tại
    int animationIdle_count = 0; // có bao nhiêu animation đứng yên
    int animationRun_count = 0; // có bao nhiêu animation chạy
    int framecountIdle = 0,framecountRun = 0; // có bao nhiêu frame
    int curFrameCount = 0; // đang ở frame của run idle .....
    void reset(int x,int y) {
        health = 1;
        point = 0;
        cnt_bom = 0;
        weaponss.clear();
        dstRect.x = x;
        dstRect.y = y;
        curFrameCount = 0;
    }
    SDL_Texture* LoadTexture(const char* path, int& spriteSheetWidth, int& spriteSheetHeight,SDL_Renderer* render);
    void is_renderer(SDL_Renderer* renderer);
    void handleInput(bool& stop);
    void render_Player(SDL_Renderer* render);
    void render_update(SDL_Renderer* render);
    void Up_All(SDL_Renderer* render,Background& bg,float deltaTime,SoundManager& sound);
    void update(Background& bg,SDL_Renderer* render,SoundManager& sound);
    void skill(Background& bg,SDL_Renderer* render,SoundManager& sound);
//    void update(Background& bg);
    int alive() {
        return health;
    }
    int Power_bom() {
        return weaponss[0].Power_Bom();
    }
    void up_power_bom() {
        for(auto &weapons : weaponss) {
            weapons.update_power();
        }
    }
    void down_alive() {
        health --;
    }
    void up_alive() {
        health ++;
    }
    void update_power() {
        for(auto weapons : weaponss) {
            weapons.update_power();
        }
    }
    void change_gun(){
        weaponss[0].change_Gun();
    }
};

#endif // PLAYER
