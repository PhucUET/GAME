#include<iostream>
#include "background.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>
#include <thread>
#include <chrono>
#include <random>

#define ii pair<int,int>
#define pii pair<int,ii>
using namespace std;

void Background::print_Lose(SDL_Renderer* render) {
    SDL_Rect dst;
    dst.w = window_width;
    dst.h = window_height;
    dst.x = 0;
    dst.y = 0;
    SDL_Texture* texture = getTileTexture(49494949,render);
    SDL_RenderCopy(render,texture,NULL,&dst);
}
void Background::huong_dan(SDL_Renderer* render) {
    SDL_Rect dst;
    dst.w = window_width;
    dst.h = window_height;
    dst.x = 0;
    dst.y = 0;
    SDL_Texture* texture = getTileTexture(11111111,render);
    SDL_RenderCopy(render,texture,NULL,&dst);
}

void Background::print_Win(SDL_Renderer* render) {
    SDL_Rect dst;
    dst.w = window_width;
    dst.h = window_height;
    dst.x = 0;
    dst.y = 0;
    SDL_Texture* texture = getTileTexture(53535353,render);
    SDL_RenderCopy(render,texture,NULL,&dst);
}
void Background::print_UI(SDL_Renderer* render) {
    SDL_Rect dst;
    dst.w = 3 * size_texture;
    dst.h = window_height;
    dst.x = 0;
    dst.y = 0;
    SDL_Texture* texture = getTileTexture(123,render);
    SDL_RenderCopy(render,texture,NULL,&dst);
}

void Background::print_nen(SDL_Renderer* render) {
    SDL_Rect dst;
    dst.w = window_width;
    dst.h = window_height;
    dst.x = 0;
    dst.y = 0;
    SDL_Texture* texture = getTileTexture(787878,render);
    SDL_RenderCopy(render,texture,NULL,&dst);
}
void Background::print_choosen_map(SDL_Renderer* render) {
    SDL_Rect dst;
    dst.w = window_width;
    dst.h = window_height;
    dst.x = 0;
    dst.y = 0;
    SDL_Texture* texture = getTileTexture(808080,render);
    SDL_RenderCopy(render,texture,NULL,&dst);
}
void Background::print_choosen_level(SDL_Renderer* render) {
    SDL_Rect dst;
    dst.w = window_width;
    dst.h = window_height;
    dst.x = 0;
    dst.y = 0;
    SDL_Texture* texture = getTileTexture(797979,render);
    SDL_RenderCopy(render,texture,NULL,&dst);
}
void Background::print_choosen_enemy(SDL_Renderer* render) {
    SDL_Rect dst;
    dst.w = window_width;
    dst.h = window_height;
    dst.x = 0;
    dst.y = 0;
    SDL_Texture* texture = getTileTexture(818181,render);
    SDL_RenderCopy(render,texture,NULL,&dst);
}
Background::Background(int type,int sz,int width,int height) {
    window_width = width;
    window_height = height;
    size_texture = sz;
    now_type = type;
    stx = 3;
    sty = 0;
    endx = window_width/size_texture;
    endy = window_height/size_texture;
}
void Background::typemap(int id) {
    int cnt = 1;
    for(int i = stx ; i < endx ; i ++) {
        for(int j = sty ; j < endy ; j ++) {
            sizemap[i][j].push_back(id * 20 + cnt);
            matrix[i][j].push_back(0);
        }
    }
//    ++cnt;
//    for(int i = stx ; i < endx ; i ++) {
//        sizemap[i][sty].push_back(id * 20 + cnt);
//    }
//    ++cnt;
//    for(int i = stx ; i < endx ; i ++) {
//        sizemap[i][endy - 1].push_back(id * 20 + cnt);
//    }
//    ++cnt;
//    for(int i = sty ; i < endy ; i ++) {
//        sizemap[stx][i].push_back(id * 20 + cnt);
//    }
//    ++cnt;
//    for(int i = sty ; i < endy ; i ++) {
//        sizemap[endx - 1][i].push_back(id * 20 + cnt);
//    }
//    sizemap[stx][sty].push_back(id * 20 + (++cnt));
//    sizemap[endx - 1][sty].push_back(id * 20 + (++cnt));
//    sizemap[stx][endy - 1].push_back(id * 20 + (++cnt));
//    sizemap[endx - 1][endy - 1].push_back(id * 20 + (++cnt));
}
void Background::loadMap(const char* path) {
    ifstream file(path);
    if(!file){
        cerr << "Khong the mo file\n" << " " << path <<"\n";
        return;
    }
    int row = sty;
    string line;
    while(getline(file,line)) {
        stringstream ss(line);
        int col = stx;
        string cell;
        while(getline(ss,cell,',') && col < endx) {
            int num = 0;
            for(int i = 0 ; i <  cell.size() ; i ++) {
                if(cell[i] > '9' || cell[i] < '0') continue;
                num = num * 10 + cell[i] - '0';
            }
            if(num > 0) sizemap[col][row].push_back(num);
            if(num >= 100) matrix[col][row].push_back(1);
            ++ col;
        }
        ++row;
    }
    file.close();
}
SDL_Texture* Background::Loadtexture(const char* path,SDL_Renderer* render) {
    SDL_Texture* texture = IMG_LoadTexture(render,path);
    if(!texture) {
        cerr <<"Khong the load texture" <<" " <<path <<"\n";
    }
    return texture;
}

SDL_Texture* Background::getTileTexture(int id,SDL_Renderer* render) {
    if(!tileTexture.count(id)) {
        string filename = to_string(id) + ".png";
        SDL_Texture* texture = Loadtexture(filename.c_str(),render);
        if(texture) {
            tileTexture[id] = texture;
        } else {
            cerr <<"khong the load texture cho id " <<" "<<id <<"\n";
            return nullptr;
        }
    }
    return tileTexture[id];
}

void Background::rendertexture(SDL_Texture* texture,int sx,int sy,SDL_Renderer* render) {
    if(!texture) return;
    SDL_Rect dst;
    dst.x = sx;
    dst.y = sy;
    dst.w = size_texture;
    dst.h = size_texture;
    SDL_RenderCopy(render,texture,NULL,&dst);
}

void Background::BackgroundD(SDL_Renderer* render) {
    for(int i = stx ; i < endx ; i ++) {
        for(int j = sty ; j < endy ; j ++) {
            for(int type : sizemap[i][j]) {
                SDL_Texture* texture = getTileTexture(type,render);
                if(texture) {
                    rendertexture(texture,i * size_texture,j * size_texture,render);
                }
            }
        }
    }
    print_UI(render);
}

bool Background::canwalk(int sx,int sy) {
    int u = (sx + 16) / size_texture;
    int v = (sy + 16) / size_texture;
    if(u < 0 || v < 0) return false;
    if(u < stx || u >= endx || v < sty || v >= endy ) return false;
    for(int type : matrix[u][v]) if(type == 1) return false;
    return true;
}

bool Background::canwalk_Enemy(int sx,int sy) {
    int u = (sx + 16) / size_texture;
    int v = (sy + 16) / size_texture;
    if(u < 0 || v < 0) return false;
    return !(matrix[u][v].back());
//    for(int type : matrix[u][v]) if(type >= 1) return false;
//    return true;
}

mt19937_64 rds(chrono::steady_clock::now().time_since_epoch().count());
int random_item(int l = 0, int r = 7) { return uniform_int_distribution<int>(l, r)(rds);}

void Background::del_pos(int sx,int sy) {
    int u = sx/size_texture;
    int v = sy/size_texture;
    if(u < 0 || v < 0) return;
    if(sizemap[u][v].back() == 100) {
        sizemap[u][v].pop_back();if(matrix[u][v].size() > 1)matrix[u][v].pop_back();
        int id = random_item();
        if(id >= 5) id = 0;
        if(id) {
            if(matrix[u][v].back() != 0) if(matrix[u][v].size() > 1) matrix[u][v].pop_back();
            sizemap[u][v].push_back(id * 1000000);
        }
    }
    else if(matrix[u][v].back() == 2) if(matrix[u][v].size() > 1) matrix[u][v].pop_back();
}

void Background::block_tile(int sx,int sy,int c) {
    int u = sx/size_texture;
    int v = sy/size_texture;
    if(u < 0 || v < 0) return;
    if(c == 0) {
        if(matrix[u][v].size() > 1) matrix[u][v].pop_back();
        return;
    }
    if(sizemap[u][v].back() != 200) matrix[u][v].push_back(c);
}

pair<int,int> Background::dijsktra(int sx,int sy) {
    vector<vector<int>> d(100,vector<int>(100,100000));
    vector<vector<pair<int,int>>> par(100,vector<pair<int,int>>(100,{-1,-1}));
    priority_queue<pii,vector<pii>,greater<pii>> pq;
    int xx = (sx + 16) / size_texture;
    int yy = (sy + 16) / size_texture;
    d[xx][yy] = 0;
    pq.push({0,{xx,yy}});
    pii mina = {100000,{-1,-1}};
    while(!pq.empty()) {
        int val = pq.top().first;
        int x = pq.top().second.first;
        int y = pq.top().second.second;
        pq.pop();
        if(matrix[x][y].back() == 0) {
            if(mina.first > val) {
                mina.second = make_pair(x,y);
                mina.first = val;
                break;
            }
        }
        for(int i = 0 ; i < 4 ; i ++) {
            int u = x + dx[i];
            int v = y + dy[i];
            if(!canwalk(u * 32 - 16,v * 32 - 16)) continue;
            if(d[u][v] > val + 1) {
                d[u][v] = val + 1;
                par[u][v] = make_pair(x,y);
                pq.push({d[u][v],{u,v}});
            }
        }
    }
    if(mina.first == 100000) {
        return make_pair(xx,yy);
    }
    while(par[mina.second.first][mina.second.second] != make_pair((sx + 16)/32,(sy + 16)/32)) {
            mina.second = par[mina.second.first][mina.second.second];
    }
//    cout << (sx + 16)/32 <<" "<<(sy + 16)/32 <<" "  <<"duong di tiep theo la    " << mina.second.first <<" "<<mina.second.second <<"\n";
    return mina.second;
}

bool Background::check_dangerous(int sx,int sy) {
    sx = (sx + 16) / size_texture;
    sy = (sy + 16) / size_texture;
    if(sx < 0 || sy < 0) return false;
    if(matrix[sx][sy].back() != 0) return true;
    return false;
}
bool Background::check_tile(int sx,int sy) {
    sx = (sx + 16)/size_texture;
    sy = (sy + 16)/size_texture;
    if(sx < 0 || sy < 0) return false;
    for(int i = 0 ; i < 4 ; i ++) {
        int u = sx + dx[i];
        int v = sy + dy[i];
        for(int type : sizemap[u][v]) {
            if(type == 100) return true;
        }
    }
    return false;
}
void Background::up_death(int sx,int sy) {
    int u = sx/size_texture;
    int v = sy/size_texture;
    if(u < 0 || v < 0) return;
    death[u][v] = 1;
}

bool Background::is_death(int sx,int sy) {
    int u = (sx + 16)/size_texture;
    int v = (sy + 16)/size_texture;
    if(u < 0 || v < 0) return false;
    return death[u][v];
}
void Background::reset_death() {
    for(int i = stx ; i < endx ; i ++) {
        for(int j = sty ; j < endy ; j ++) {
            death[i][j] = 0;
        }
    }
}

void Background::update_character(int sx,int sy,bool c) {
    int u = (sx + 16)/size_texture;
    int v = (sy + 16)/size_texture;
    character[u][v] = c;
}

bool Background::check_character(int sx,int sy) {
    int u = sx/size_texture;
    int v = sy/size_texture;
    return character[u][v];
}
bool Background::check_destroy(int sx,int sy) {
    int u = sx/size_texture;
    int v = sy/size_texture;
    if(sizemap[u][v].back() == 100) return true;
    return false;
}

int Background::check_item(int sx,int sy) {
    int u = (sx + 16)/size_texture;
    int v = (sy + 16)/size_texture;
    if(sizemap[u][v].back() % 1000000 == 0) {
        int type = sizemap[u][v].back();
        sizemap[u][v].pop_back();
        return type;
    }
    return 0;
}
