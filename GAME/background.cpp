#include<iostream>
#include "background.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

Background::Background(int type,int sz,int width,int height) {
    window_width = width;
    window_height = height;
    size_texture = sz;
    now_type = type;
    stx = (window_width - window_height)/size_texture;
    sty = 0;
    endx = window_width/size_texture;
    endy = window_height/size_texture;
}
void Background::typemap(int id) {
    int cnt = 1;
    for(int i = stx ; i < endx ; i ++) {
        for(int j = sty ; j < endy ; j ++) {
            sizemap[i][j].push_back(id * 20 + cnt);
        }
    }
    ++cnt;
    for(int i = stx ; i < endx ; i ++) {
        sizemap[i][sty].push_back(id * 20 + cnt);
    }
    ++cnt;
    for(int i = stx ; i < endx ; i ++) {
        sizemap[i][endy - 1].push_back(id * 20 + cnt);
    }
    ++cnt;
    for(int i = sty ; i < endy ; i ++) {
        sizemap[stx][i].push_back(id * 20 + cnt);
    }
    ++cnt;
    for(int i = sty ; i < endy ; i ++) {
        sizemap[endx - 1][i].push_back(id * 20 + cnt);
    }
    sizemap[stx][sty].push_back(id * 20 + (++cnt));
    sizemap[endx - 1][sty].push_back(id * 20 + (++cnt));
    sizemap[stx][endy - 1].push_back(id * 20 + (++cnt));
    sizemap[endx - 1][endy - 1].push_back(id * 20 + (++cnt));
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
            for(int i = 0 ; i <  cell.size() ; i ++) num = num * 10 + cell[i] - '0';
            if(num > 0) sizemap[row][col].push_back(num);
            if(num % 300 == 0) matrix[row][col] = 1;
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
            cerr << "tonbonhokhong" << id <<"\n";
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
}
