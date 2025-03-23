#ifndef PLAYER.H
#define PLAYER.H

#include "weapon.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "background.h"
#include <utility>
class Player{
private:
    SDL_Rect dstRect;
    SDL_Rect srcRect = { 0, 0, 64, 64};

    SDL_Texture* idleTexture = nullptr;
    SDL_Texture* runTexture = nullptr;
    SDL_Texture* currentTexture = nullptr;
    SDL_Renderer* renderer_player = nullptr;

    int speed = 32;
    int health  = 1;


    Weapon weapons;
    const Uint8* keyStates = nullptr;
    bool moving = false;
    bool isrunning = false;
    Uint32 lastTime = SDL_GetTicks();

    int dx[4] = {0,32,-32,0};
    int dy[4] = {32,0,0,-32};

public:
    Player(int sx,int sy);
    pair<int,int> location() {
        return make_pair(dstRect.x,dstRect.y);
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
    SDL_Texture* LoadTexture(const char* path, int& spriteSheetWidth, int& spriteSheetHeight,SDL_Renderer* render);
    void is_renderer(SDL_Renderer* renderer);
    void handleInput();
    void render_Player(SDL_Renderer* render);
    void render_update(SDL_Renderer* render);
    void Up_All(SDL_Renderer* render,Background& bg,float deltaTime);
    void update(Background& bg,SDL_Renderer* render);
    void skill(Background& bg,SDL_Renderer* render);
//    void update(Background& bg);
    bool alive() {
        return health;
    }
    void down_alive() {
        health --;
    }
    void update_power() {
        weapons.update_power();
    }
};

#endif // PLAYER
