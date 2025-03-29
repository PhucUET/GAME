#ifndef BACKGROUND.H
#define BACKGROUND.H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <fstream>
#include <map>
#include <utility>
using namespace std;

class Background{
private:
    vector<int> matrix[200][200];
    bool character[200][200];
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
    void print_UI(SDL_Renderer* render);
    void print_nen(SDL_Renderer* render);
    void print_choosen_map(SDL_Renderer* render);
    void print_choosen_enemy(SDL_Renderer* render);
    void print_choosen_level(SDL_Renderer* render);
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
    void block_tile(int sx,int sy,int c);
    pair<int,int> dijsktra(int sx,int sy);
    bool check_dangerous(int sx,int sy);
    bool wall_check(int sx,int sy) {
        int u = sx / size_texture;
        int v = sy / size_texture;
        if(!sizemap[u][v].size()) return true;
        for(int type : sizemap[u][v]) {
            if(type % 200 == 0) return true;
        }
        return false;
    }
    bool check_tile(int sx,int sy);
    void up_death(int sx,int sy);
    bool is_death(int sx,int sy);
    void reset_death();

    void update_character(int sx,int sy,bool c);
    bool check_character(int sx,int sy);
    bool check_destroy(int sx,int sy);
    int check_item(int sx,int sy);

};
#endif // BACKGROUND
