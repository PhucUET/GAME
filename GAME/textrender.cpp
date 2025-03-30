#include "textrender.h"

TextRenderer::TextRenderer(int fontSize) {

    font_ = TTF_OpenFont("Arial.ttf",fontSize);
    if (!font_) {
        std::cerr << "Failed to load font: " << "arial.ttf" << " ! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }
}

TextRenderer::~TextRenderer() {
    if (font_) {
        TTF_CloseFont(font_);
        font_ = nullptr;
    }
}

void TextRenderer::renderText(SDL_Renderer* renderer, const std::string& text, SDL_Color color, int x, int y) {
    if (!font_ || !renderer) {
        std::cerr << "Cannot render text: Font or renderer is invalid." << std::endl;
        return;
    }
    if (text.empty()) {
        return;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font_, text.c_str(), color);
    if (!textSurface) {
        std::cerr << "Failed to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cerr << "Failed to create texture from rendered text! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return;
    }
    SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };

    SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

bool TextRenderer::isFontLoaded() const {
    return font_ != nullptr;
}
