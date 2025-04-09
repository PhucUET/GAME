# BOMIT 💣

Chào mừng đến với BOMIT, một trò chơi phong cách Bomberman cổ điển được xây dựng bằng C++ và thư viện SDL2! Đặt bom, phá hủy chướng ngại vật, thu thập vật phẩm và đánh bại đối thủ của bạn trong các màn chơi đầy thử thách.

## 📜 Giới thiệu

BOMIT là một dự án game được tạo ra với mục đích học hỏi và giải trí, lấy cảm hứng từ dòng game Bomberman huyền thoại. Người chơi điều khiển nhân vật của mình di chuyển trong một mê cung, đặt bom để phá các khối mềm, thu thập vật phẩm tăng sức mạnh và cố gắng loại bỏ đối thủ (AI hoặc người chơi khác).

## ✨ Tính năng chính

*   Lối chơi đặt bom cổ điển, đầy tính chiến thuật.
*   Chế độ chơi đơn đấu với AI.
*   Nhiều loại vật phẩm tăng sức mạnh:
    *   Tăng máu.
    *   Tăng số lượng bom có thể đặt cùng lúc.
    *   Tăng phạm vi nổ của bom.
    *   Súng laser.
*   Các khối có thể phá hủy, có thể chứa vật phẩm ngẫu nhiên.
*   Màn chơi với thiết kế khác nhau.
*   Hiệu ứng âm thanh và nhạc nền.
*   Chức năng tạm dừng (Pause) game.
*   Bộ đếm thời gian và hiển thị điểm số.

## 🎮 Cách chơi

**Mục tiêu:** Là người sống sót cuối cùng bằng cách đặt bom để loại bỏ đối thủ. Phá hủy các khối mềm để mở đường và tìm vật phẩm.

**Điều khiển:**

*   **Người chơi 1:**
    *   Di chuyển: `W`, `A`, `S`, `D`
    *   Đặt bom: `Space`
*   **Chung:**
    *   Quay lại bước đằng trước;Tạm dừng / Tiếp tục: `ESC`
    *   Sang bước tiếp theo `Space`
    *   Chọn level, màn chơi,... `1`, `2`, `3`, ...

## ⚙️ Yêu cầu hệ thống & Thư viện

Để biên dịch và chạy game, bạn cần:

*   Một trình biên dịch C++ hỗ trợ C++11 trở lên (ví dụ: g++, Clang, MSVC).
*   Thư viện **SDL2** (Simple DirectMedia Layer)
*   Thư viện **SDL2_image** (Để tải ảnh PNG, JPG, ...)
*   Thư viện **SDL2_ttf** (Để hiển thị text, font)
*   Thư viện **SDL2_mixer** (Để xử lý âm thanh, nhạc)

## 🛠️ Hướng dẫn cài đặt và biên dịch

1.  **Cài đặt các thư viện SDL2:** Đảm bảo bạn đã cài đặt đầy đủ các thư viện SDL2, SDL2_image, SDL2_ttf, và SDL2_mixer development libraries cho hệ điều hành của bạn.
    *   **Ubuntu/Debian:** `sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev`
    *   **Fedora:** `sudo dnf install SDL2-devel SDL2_image-devel SDL2_ttf-devel SDL_mixer-devel`
    *   **macOS (dùng Homebrew):** `brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer`
    *   **Windows (dùng MinGW/MSYS2):** Sử dụng `pacman` để cài đặt các gói tương ứng (ví dụ: `mingw-w64-x86_64-SDL2`, `mingw-w64-x86_64-SDL2_image`, ...) hoặc tải các development library từ trang chủ LibSDL và thiết lập trong IDE của bạn.

2.  **Clone Repository (Nếu có):**
    ```bash
    git clone [URL của repository của bạn]
    cd [Tên thư mục repository]
    ```

3.  **Biên dịch:**
    *   **Cách đơn giản dùng g++ (Linux/macOS/MinGW):**
        ```bash
        g++ src/*.cpp -o bomit -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -std=c++11
        ```
        *(Bạn có thể cần điều chỉnh đường dẫn đến các file `.cpp` và thứ tự các thư viện `-l` tùy thuộc vào cấu trúc dự án)*
    *   **Sử dụng IDE (Code::Blocks, Visual Studio, CLion,...):** Tạo một project mới, thêm các file mã nguồn (`.h`, `.cpp`), và cấu hình project để liên kết (link) với các thư viện SDL2 đã cài đặt.
    *   **Sử dụng CMake (Nếu bạn thiết lập):**
        ```bash
        mkdir build
        cd build
        cmake ..
        make
        ```

## ▶️ Chạy game

Sau khi biên dịch thành công, chạy file thực thi:

*   **Linux/macOS:** `./bomit`
*   **Windows:** `bomit.exe`


## 👤 Tác giả

*   Đinh Gia Phúc
*   0336893606

## 🙏 Cảm ơn / Nguồn tài nguyên

*   Cảm ơn thư viện SDL2 và các thư viện mở rộng.
*   Sprites từ https://craftpix.net/.org
*   Âm thanh từ https://freesound.org/
