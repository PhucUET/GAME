#include "enemy.h"
#include <SDL.h>
#include <SDL_image.h>
#include <thread>
#include <random>
#include <chrono>
#include <string>
#include "weapon.h"
const int frame_height = 64;
const int frame_width = 64;
const int Frame_delay = 100;


Enemy::Enemy(int sx,int sy,int stt) {
    dstRect.x = sx;
    dstRect.y = sy;
    dstRect.w = frame_width;
    dstRect.h = frame_height;
    cnt = stt;
    cout<< sx <<" "<<sy <<" " << cnt <<" "<<stt <<"\n";
//    idle_namefile = filename_idle[stt];
//    run_namefile = filename_walk[stt];
//    std::cout << idle_namefile <<"\n";
}

SDL_Texture* Enemy::LoadTexture(const char* path, int& spriteSheetWidth, int& spriteSheetHeight,SDL_Renderer* render) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (!loadedSurface) {
        std::cerr << "Khong the load anh! " << IMG_GetError() <<" " << path << std::endl;
        return nullptr;
    }
    newTexture = SDL_CreateTextureFromSurface(render, loadedSurface);
    spriteSheetWidth = loadedSurface->w;
    spriteSheetHeight = loadedSurface->h;
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

void Enemy::render_Player(SDL_Renderer* render) {
    up_cnt_bom();
    idleTexture = LoadTexture(filename_idle[cnt],idleWidth,idleHeight,render);
    runTexture = LoadTexture(filename_walk[cnt],runWidth,runHeight,render);
    cout << cnt <<"\n";
    if (!idleTexture || !runTexture) return;
    animationIdle_count = idleHeight / frame_height;
    animationRun_count  = runHeight / frame_height;

    currentTexture = idleTexture;
    framecountIdle = idleWidth / frame_width;
    framecountRun  = runWidth / frame_width;
}

const int FRAME_DELAY = 70;
void Enemy::render_update(SDL_Renderer* render) {
//        SDL_Delay(100);
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


void Enemy::skill(Background& bg,SDL_Renderer* render,SoundManager& sound) {
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
            sound.playSoundEffect("laser",0);
            if(direct == 0 || direct == 3)texture = bg.getTileTexture(88888,render);
            else texture = bg.getTileTexture(888888,render),direct = direct;
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

const int RUN_DELAY = 400;
mt19937_64 rd(chrono::steady_clock::now().time_since_epoch().count());
int random_direction(int l = 0, int r = 3) { return uniform_int_distribution<int>(l, r)(rd);}
int random_yn(int l = 0, int r = 1) {return uniform_int_distribution<int>(l,r)(rd);}
void Enemy::nextMove(SDL_Renderer* render,Background& bg) {
    Uint32 Now = SDL_GetTicks();
    if(Now > RunTime + RUN_DELAY) {
        RunTime = Now;
    } else return;
    if(bg.check_dangerous(dstRect.x,dstRect.y)) {
        pair<int,int> new_pos = bg.dijsktra(dstRect.x,dstRect.y);
        for(int i = 0 ; i < 4 ; i ++) {
            if(new_pos.first * 32 - 16 - dstRect.x == dx[i] && new_pos.second * 32 - 16 - dstRect.y == dy[i]) {
                currentAnimation = i;
                dstRect.x = new_pos.first * 32 - 16;
                dstRect.y = new_pos.second * 32 - 16;
            }
        }
    } else {
        int id = random_direction();
        while(3 - id == currentAnimation && bg.canwalk_Enemy(dstRect.x + dx[currentAnimation],dstRect.y + dy[currentAnimation])) {
            id =random_direction();
        }
        cout << dstRect.x + dx[id] <<" "<<dstRect.y + dy[id] <<" " << bg.canwalk_Enemy(dstRect.x + dx[id],dstRect.y + dy[id]) <<"\n";
        if(bg.canwalk_Enemy(dstRect.x + dx[id],dstRect.y + dy[id])) {
            currentAnimation = id;
            dstRect.x = dstRect.x + dx[id];
            dstRect.y = dstRect.y + dy[id];
        }
    }
}
void Enemy::Up_All(SDL_Renderer* render, Background& bg,float deltaTime,SoundManager& sound) {
    for(auto &weapons : weaponss) {
//            cout<< weapons.check_bom() <<" " <<"da dat bom ver2 \n";
//            cout << weapons.check_bom() <<" "<<weaponss.size() <<" " <<"dat duoc bom roi\n";
            if(weapons.check_bom()) {
                weapons.update(deltaTime,render);
                if(!weapons.check_bom()) {
                    pair<int,int> pos = weapons.pos_bom();
                    int u = pos.first;
                    int v = pos.second;
                    SDL_Texture* texture = nullptr;
                    sound.playSoundEffect("bom",0);
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

    if(bg.check_tile(dstRect.x,dstRect.y)) {
        int c = random_direction();
        if(c == 2 || bg.check_character(dstRect.x + 16,dstRect.y + 16)) {
           skill(bg,render,sound);
        }
    }
//    nextMove(render,bg);
    render_update(render);
    return;
}
