#include "player.h"
#include <SDL.h>
#include <SDL_image.h>
#include <thread>
#include "weapon.h"
#define idle_namefile "Vampires1_Idle_full.png"
#define run_namefile "Vampires1_Walk_full.png"

using namespace std;
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
    up_cnt_bom();
    idleTexture = LoadTexture(idle_namefile,idleWidth,idleHeight,render);
    runTexture = LoadTexture(run_namefile,runWidth,runHeight,render);
    if (!idleTexture || !runTexture) return;
    animationIdle_count = idleHeight / frame_height;
    animationRun_count  = runHeight / frame_height;

    currentTexture = idleTexture;
    framecountIdle = idleWidth / frame_width;
    framecountRun  = runWidth / frame_width;
}
void Player::update(Background& bg,SDL_Renderer* render,SoundManager& sound) {
     moving = false; // Mặc định là không di chuyển
    double newX = dstRect.x;
    double newY = dstRect.y;
    if (key_w) {
        newY -= speed;
        currentAnimation = 1;
        moving = true;
    } else if (key_s) { // Sử dụng else if
        newY += speed;
        currentAnimation = 0;
        moving = true;
    } else if (key_d) {
        newX += speed;
        currentAnimation = 3;
        moving = true;
    } else if (key_a) {
        newX -= speed;
        currentAnimation = 2;
        moving = true;
    }
    if (key_space) {
            skill(bg,render,sound);
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
//        currentAnimation = 0; // Về idle
        currentTexture = idleTexture;
        curFrameCount = framecountIdle;
        isrunning = false;
    }
    if(bg.canwalk(newX,newY)) {
        dstRect.x = newX;
        dstRect.y = newY;
    }
}

void Player::handleInput(bool &stop) {
   SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_w:
                key_w = true;
                break;
            case SDLK_s:
                key_s = true;
                break;
            case SDLK_a:
                key_a = true;
                break;
            case SDLK_d:
                key_d = true;
                break;
            case SDLK_SPACE:
                key_space = true;
                break;
            case SDLK_ESCAPE:
                key_esc = true;
            }
        } else if(e.type == SDL_KEYUP) {
            switch (e.key.keysym.sym) {
            case SDLK_w:
                key_w = false;
                break;
            case SDLK_s:
                key_s = false;
                break;
            case SDLK_a:
                key_a = false;
                break;
            case SDLK_d:
                key_d = false;
                break;
            case SDLK_SPACE:
                key_space = false;
                break;
            }
        }
    }
    if(key_esc) {
        if(!stop) stop = true;
        else stop = false;
        key_esc = false;
    }
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

void Player::Up_All(SDL_Renderer* render, Background& bg,float deltaTime,SoundManager& sound) {
    for(auto &weapons : weaponss) {
//            cout<< weapons.check_bom() <<" " <<"da dat bom ver2 \n";
//            cout << weapons.check_bom() <<" "<<weaponss.size() <<" " <<"dat duoc bom roi\n";
            if(weapons.check_bom()) {
                weapons.update(deltaTime,render);
                if(!weapons.check_bom()) {
                    sound.playSoundEffect("bom",0);
                    pair<int,int> pos = weapons.pos_bom();
                    int u = pos.first;
                    int v = pos.second;
                    SDL_Texture* texture = nullptr;
                    bg.up_death(u,v);
                    bg.block_tile(u,v,0);
                    texture = bg.getTileTexture(99999,render);
                    bg.rendertexture(texture, u, v,render);
                    for(int i = 0 ; i < 4 ; i ++) {
                        for(int k = 1 ; k <= weapons.Power_Bom() ; k ++) {
                            if(bg.wall_check(u + dx[i] * k,v + dy[i] * k)) break;
                            if(bg.check_destroy(u + dx[i] * k,v + dy[i] * k)) {
                                point += 50;
                            }
                            if(bg.check_character(u + dx[i] * k,v + dy[i] * k)) {
                                point += 500;
                            }
                            bg.rendertexture(texture, u + dx[i] * k, v + dy[i] * k,render);
                            bg.up_death(u + dx[i] * k,v + dy[i] * k);
                            bg.block_tile(u + dx[i] * k,v + dy[i] * k,0);
                            if(bg.check_destroy(u + dx[i] * k,v + dy[i] * k)) {
                                bg.del_pos(u + dx[i] * k,v + dy[i] * k);
                                break;
                            }
                        }
                    }
    //            for(int i = 0 ; i < 4 ; i ++) {
    //                for(int k = 1 ; k <= weapons.Power_Bom() ; k ++) {
    //                    bg.block_tile(u + k * dx[i],v + k * dy[i],0);
    //                }
    //            }
            }
        }
    }
    update(bg,render,sound);
    render_update(render);
}

void Player::skill(Background& bg,SDL_Renderer* render,SoundManager& sound) {
    int sx = dstRect.x + 16;
    int sy = dstRect.y + 16;
//    if(point == 0)weaponss[0].change_Gun();
    if(!weaponss.size()) return;
    weaponss[0].check_type();
    int cnt = 0;
    for(auto &weapons: weaponss) {
        if(weapons.Bom()) {
            if(weapons.check_bom()) continue;
            bg.block_tile(sx,sy,1);
            for(int i = 0 ; i < 4 ; i ++) {
                for(int k = 1 ; k <= weapons.Power_Bom() ; k ++) {
                    if(bg.wall_check(sx + dx[i] * k,sy + dy[i] * k) || bg.check_destroy(sx + dx[i] * k,sy + dy[i] * k)) break;
                    bg.block_tile(sx + k * dx[i],sy + k * dy[i],2);
                }
            }
            weapons.render_Bom(render,sx,sy);
            break;
        }
        if(weapons.Gun())  {
            SDL_Texture* texture = nullptr;
            int direct = currentAnimation;
            if(direct == 0 || direct == 1)texture = bg.getTileTexture(88888,render);
            else texture = bg.getTileTexture(888888,render),direct = direct;
            sound.playSoundEffect("laser",0);
            while(!bg.wall_check(sx = sx + dx[direct],sy = sy + dy[direct])) {
                bg.rendertexture(texture,sx,sy,render);
                bg.up_death(sx,sy);
                bg.up_death(sx,sy);
                if(bg.check_character(sx,sy)) point += 500;
                if(bg.check_destroy(sx,sy)) {
                    point += 50;
                    bg.del_pos(sx,sy);
                    break;
                }
            }
            break;
        }
    }
}
