#include "player.h"
#include <SDL.h>
#include <SDL_image.h>
#include <thread>
#include "weapon.h"
#define idle_namefile "Sword_Idle_full.png"
#define run_namefile "Sword_Walk_full.png"
const int frame_height = 64;
const int frame_width = 64;
const int Frame_delay = 100;

Player::Player(int sx,int sy) {
    dstRect.x = sx;
    dstRect.y = sy;
    dstRect.w = frame_width;
    dstRect.h = frame_height;
}

SDL_Texture* Player::LoadTexture(const char* path, int& spriteSheetWidth, int& spriteSheetHeight,SDL_Renderer* render) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (!loadedSurface) {
        std::cerr << "Không thể load ảnh! " << IMG_GetError() << std::endl;
        return nullptr;
    }
    newTexture = SDL_CreateTextureFromSurface(render, loadedSurface);
    spriteSheetWidth = loadedSurface->w;
    spriteSheetHeight = loadedSurface->h;
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

void Player::render_Player(SDL_Renderer* render) {
    idleTexture = LoadTexture(idle_namefile,idleWidth,idleHeight,render);
    runTexture = LoadTexture(run_namefile,runWidth,runHeight,render);
    if (!idleTexture || !runTexture) return;
    animationIdle_count = idleHeight / frame_height;
    animationRun_count  = runHeight / frame_height;

    currentTexture = idleTexture;
    framecountIdle = idleWidth / frame_width;
    framecountRun  = runWidth / frame_width;
}
void Player::update(Background& bg,SDL_Renderer* render) {
     moving = false; // Mặc định là không di chuyển
    double newX = dstRect.x;
    double newY = dstRect.y;
    if (keyStates[SDL_SCANCODE_W]) {
        newY -= speed;
        currentAnimation = 3;
        moving = true;
    } else if (keyStates[SDL_SCANCODE_S]) { // Sử dụng else if
        newY += speed;
        currentAnimation = 0;
        moving = true;
    } else if (keyStates[SDL_SCANCODE_D]) {
        newX += speed;
        currentAnimation = 2;
        moving = true;
    } else if (keyStates[SDL_SCANCODE_A]) {
        newX -= speed;
        currentAnimation = 1;
        moving = true;
    }
        if (keyStates[SDL_SCANCODE_SPACE]) {
            if(!weapons.check_bom()) skill(bg,render); //Sửa lại skill
    }

    if (moving) {
        if (!isrunning) {
            frame_count = 0;
            currentTexture = runTexture;
             curFrameCount = framecountRun;
            isrunning = true;
        }
    } else {
        if (isrunning) {
            frame_count = 0;
        }
        currentAnimation = 0; // Về idle
        currentTexture = idleTexture;
        curFrameCount = framecountIdle;
        isrunning = false;
    }
    if(bg.canwalk(newX,newY)) {
        dstRect.x = newX;
        dstRect.y = newY;
    }
}

void Player::handleInput() {
   keyStates = SDL_GetKeyboardState(nullptr);
}
const int FRAME_DELAY = 70;
void Player::render_update(SDL_Renderer* render) {
        SDL_Delay(100);
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastTime + FRAME_DELAY) {
            frame_count++;
            if (frame_count >= curFrameCount) {
                frame_count = 0;
            }
            lastTime = currentTime;
        }
        srcRect.x = frame_count * frame_width;
        srcRect.y = currentAnimation * frame_height;

//        (renderer_player);
        SDL_RenderCopy(render, currentTexture, &srcRect, &dstRect);
//        SDL_RenderPresent(renderer_player);
}

void Player::Up_All(SDL_Renderer* render, Background& bg,float deltaTime) {
    if(weapons.check_bom()) {
        weapons.update(deltaTime,render);
        if(!weapons.check_bom()) {
            pair<int,int> pos = weapons.pos_bom();
            int u = pos.first;
            int v = pos.second;
            bg.del_pos(u,v);
            bg.up_death(u,v);
            for(int i = 0 ; i < 4 ; i ++) {
                for(int k = 1 ; k <= weapons.Power_Bom() ; k ++) {
                    bg.del_pos(u + dx[i] * k,v + dy[i] * k);
                    bg.up_death(u + dx[i] * k,v + dy[i] * k);
                }
            }
//            for(int i = 0 ; i < 4 ; i ++) {
//                for(int k = 1 ; k <= weapons.Power_Bom() ; k ++) {
//                    bg.block_tile(u + k * dx[i],v + k * dy[i],0);
//                }
//            }
        }
    }
    update(bg,render);
    render_update(render);
}

void Player::skill(Background& bg,SDL_Renderer* render) {
    int sx = dstRect.x + 16;
    int sy = dstRect.y + 16;
    if(weapons.Bom()) {
        bg.block_tile(sx,sy);
        for(int i = 0 ; i < 4 ; i ++) {
            for(int k = 1 ; k <= 1 + weapons.Power_Bom() ; k ++) {
                bg.block_tile(sx + k * dx[i],sy + k * dy[i]);
            }
        }
        weapons.render_Bom(render,sx,sy);
    }
}
