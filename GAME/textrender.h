#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream> // Để in lỗi

class TextRenderer {
public:
    // Constructor: Load font khi tạo đối tượng
    // fontPath: Đường dẫn đến file font (.ttf)
    // fontSize: Kích thước font mong muốn
    TextRenderer(int fontSize);

    // Destructor: Giải phóng font khi đối tượng bị hủy
    ~TextRenderer();

    // Hàm để vẽ text lên màn hình
    // renderer: SDL_Renderer để vẽ lên
    // text: Chuỗi text cần vẽ
    // color: Màu chữ (SDL_Color)
    // x, y: Tọa độ góc trên bên trái của text
    void renderText(SDL_Renderer* renderer, const std::string& text, SDL_Color color, int x, int y);

    // Hàm kiểm tra xem font đã được load thành công chưa
    bool isFontLoaded() const;

private:
    TTF_Font* font_ = nullptr; // Con trỏ tới font đã load
};

#endif // TEXTRENDERER_H
