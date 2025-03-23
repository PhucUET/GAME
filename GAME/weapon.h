#ifndef WEAPON.H
#define WEAPON.H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <utility>

using namespace std;
class Weapon{
private:
    bool have_bom = false;
    int frameCount_weapon = 0;
    SDL_Rect src = { 0, 0, 32, 32};
    SDL_Rect dst;
    enum WeaponType {BOM = 1, GUN = 2, EXTRA_BOOM = 3};
    WeaponType Weatype = BOM;
    int times = 2;
    int power = 1;
    bool isExploding = false;
    float timeToExplode = 5000.0f;
    float explosionTime = 0.0f;
    float maxExplosionTime = 0.5f;
    Uint32 lastTime = SDL_GetTicks();
    SDL_Texture* texture_Bom = nullptr;


public:
    bool check_bom();
    int frame_width = 32;
    int frame_count = 0;
    pair<int,int> pos_bom() {
        return make_pair(dst.x,dst.y);
    }
    int frame_count_bom = 0; // hiện tại đang ở frame bao nhiêu của bom
    int framecountBom = 0; // có bao nhiêu frame
    int curFrameCount = 0;
    int Bom_Width = 32;
    int Bom_Height = 32;
    void render_Bom(SDL_Renderer* render,int sx,int sy);
    void render_update(SDL_Renderer* render);
    SDL_Texture* loadTexture(const char* path, int& spriteSheetWidth, int& spriteSheetHeight,SDL_Renderer* render);
    void update(float deltaTime,SDL_Renderer* render);
    bool Bom();
    WeaponType GetWeapon() const{ return Weatype;}
    int Power_Bom() {
        return power;
    }
    void update_power() {
        power ++;
    }

};

#endif // WEAPON
