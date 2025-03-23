#ifndef BACKGROUND.H
#define BACKGROUND.H

#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include <vector>
#include <fstream>
#include <map>
#include <utility>
using namespace std;

class Background{
private:
    int matrix[200][200];
    bool death[200][200];
    vector<int> sizemap[200][200];
    map<int,SDL_Texture*> tileTexture;
    int dx[4] = {1,0,-1,0};
    int dy[4] = {0,1,0,-1};

    int window_width = 640;
    int window_height = 480;

    int size_texture = 32;
    int now_type = 0;

    int stx = 3;
    int sty = 0;
    int endx = window_width/size_texture;
    int endy = window_height/size_texture;


public:
    Background(int type, int sz,int width,int height);
    void typemap(int id);
    void loadMap(const char* path);
    SDL_Texture* Loadtexture(const char* path, SDL_Renderer* render);
    SDL_Texture* getTileTexture(int id, SDL_Renderer* render);
    void rendertexture(SDL_Texture* texture,int sx,int sy,SDL_Renderer* render);
    void BackgroundD(SDL_Renderer* render);
    bool canwalk(int sx,int sy);
    bool canwalk_Enemy(int sx,int sy);
    void del_pos(int sx,int sy);
    void block_tile(int sx,int sy);
    pair<int,int> dijsktra(int sx,int sy);
    bool check_dangerous(int sx,int sy);
    bool check_tile(int sx,int sy);
    void up_death(int sx,int sy);
    bool is_death(int sx,int sy);
    void reset_death();
};
#endif // BACKGROUND
