#include<iostream>
#include "weapon.h"

using namespace std;

SDL_Texture* Weapon::loadTexture(const char* path, int& spriteSheetWidth, int& spriteSheetHeight,SDL_Renderer* render) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path);
    if(!loadedSurface) {
        cerr << "Khong the load anh!" <<" "<<IMG_GetError() << "\n";
        return nullptr;
    }
    newTexture = SDL_CreateTextureFromSurface(render,loadedSurface);
    spriteSheetWidth = loadedSurface->w;
    spriteSheetHeight = loadedSurface->h;
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}
const int FRAME_DELAY = 70;
void Weapon::update(float deltaTime,SDL_Renderer* render) {
    if(!isExploding) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastTime + FRAME_DELAY) {
            frame_count++;
            if (frame_count >= framecountBom) {
                frame_count = 0;
            }
            lastTime = currentTime;
        }
        src.x = frame_count * frame_width;
        src.y = 0;
        SDL_RenderCopy(render,texture_Bom,&src,&dst);
        timeToExplode -= deltaTime * 1000;
        if(timeToExplode <= 0) {
             have_bom = false;
            explosionTime += deltaTime;
            isExploding = false;
            frame_count = 0;
            timeToExplode = 2000.0f;
        }
    }
}
void Weapon::render_Bom(SDL_Renderer* render,int sx,int sy) {
    have_bom = true;
    dst.x = sx;
    dst.y = sy;
    dst.h = frame_width;
    dst.w = frame_width;
    int type = Weatype;
    type = type * 10000;
    string wea = to_string(type) + ".png";
    SDL_Texture* texture = loadTexture(wea.c_str(),Bom_Width,Bom_Height,render);
    framecountBom = Bom_Width/frame_width;
//    cout << sx <<" "<<sy <<"\n";
    texture_Bom = texture;
}
bool Weapon::check_bom() {
    return have_bom;
}
bool Weapon::Bom() {
    if(GetWeapon() == BOM) return true;
    return false;
}
void Weapon::check_type() {
    if(times == 0) {
        change_Bom();
        times = 999999;
    }
    times = times - 1;
}
//void Weapon::render_shoot(SDL_Renderer* render, Background& bg, int direct, int sx, int sy) {
//
//}
