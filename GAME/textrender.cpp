#include "textrender.h"

// Constructor
TextRenderer::TextRenderer(int fontSize) {
    // Load font từ đường dẫn và kích thước đã cho
    font_ = TTF_OpenFont("Arial.ttf",fontSize);
    if (!font_) {
        // In lỗi nếu không load được font
        std::cerr << "Failed to load font: " << "arial.ttf" << " ! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }
}

// Destructor
TextRenderer::~TextRenderer() {
    // Đóng font nếu nó đã được load
    if (font_) {
        TTF_CloseFont(font_);
        font_ = nullptr; // Đặt lại con trỏ để tránh lỗi double free
    }
}

// Hàm vẽ text
void TextRenderer::renderText(SDL_Renderer* renderer, const std::string& text, SDL_Color color, int x, int y) {
    // Kiểm tra xem font và renderer có hợp lệ không
    if (!font_ || !renderer) {
        std::cerr << "Cannot render text: Font or renderer is invalid." << std::endl;
        return;
    }
    if (text.empty()) {
        return; // Không vẽ nếu text rỗng
    }
    // Tạo surface từ text, font và màu sắc
    // TTF_RenderText_Solid: Nhanh, không có blending
    // TTF_RenderText_Blended: Chậm hơn, có alpha blending (đẹp hơn)
    // TTF_RenderText_Shaded: Nền màu, có blending
    SDL_Surface* textSurface = TTF_RenderText_Solid(font_, text.c_str(), color);
    if (!textSurface) {
        std::cerr << "Failed to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    // Tạo texture từ surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cerr << "Failed to create texture from rendered text! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface); // Giải phóng surface ngay cả khi lỗi tạo texture
        return;
    }

    // Lấy kích thước của texture (dựa trên kích thước surface)
    SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };

    // Vẽ texture lên màn hình
    SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

    // Giải phóng texture và surface (rất quan trọng!)
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

// Hàm kiểm tra font
bool TextRenderer::isFontLoaded() const {
    return font_ != nullptr;
}
