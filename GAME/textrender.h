#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream> // Để in lỗi

class TextRenderer {
public:
    TextRenderer(int fontSize);
    ~TextRenderer();
    void renderText(SDL_Renderer* renderer, const std::string& text, SDL_Color color, int x, int y);
    bool isFontLoaded() const;

private:
    TTF_Font* font_ = nullptr;
};

#endif // TEXTRENDERER_H
