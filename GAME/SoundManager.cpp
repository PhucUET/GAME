
#include "SoundManager.h"

// Constructor
SoundManager::SoundManager() {
    // Khởi tạo SDL_mixer với các định dạng âm thanh mong muốn (ví dụ: MP3, OGG)
    // Bạn có thể bỏ qua Mix_Init nếu chỉ dùng WAV, nhưng nên có để hỗ trợ nhiều định dạng
    int flags = MIX_INIT_MP3 | MIX_INIT_OGG; // Thêm các định dạng khác nếu cần
    int initted = Mix_Init(flags);
    if ((initted & flags) != flags) {
        std::cerr << "Mix_Init: Failed to init required ogg and mp3 support!\n";
        std::cerr << "Mix_Init: " << Mix_GetError() << std::endl;
        // Không cần thoát ở đây, Mix_OpenAudio có thể vẫn hoạt động với WAV
    }

    // Mở thiết bị âm thanh
    // Tần số, Định dạng, Số kênh (2=stereo), Kích thước chunk (quan trọng cho độ trễ)
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! Mix_Error: " << Mix_GetError() << std::endl;
        initialized_ = false;
    } else {
        initialized_ = true;
        std::cout << "SDL_mixer initialized successfully!" << std::endl;
        // Đặt âm lượng mặc định (ví dụ)
        setSoundEffectsVolume(MIX_MAX_VOLUME / 2); // 50% volume
        setMusicVolume(MIX_MAX_VOLUME / 2);
    }
}

// Destructor
SoundManager::~SoundManager() {
    // Giải phóng tất cả sound effects
    for (auto const& [id, chunk] : soundEffects_) {
        if (chunk) {
            Mix_FreeChunk(chunk);
        }
    }
    soundEffects_.clear(); // Xóa map

    // Giải phóng nhạc nền
    if (backgroundMusic_) {
        Mix_FreeMusic(backgroundMusic_);
        backgroundMusic_ = nullptr;
    }

    // Đóng thiết bị âm thanh
    Mix_CloseAudio();

    // Thoát SDL_mixer
    while (Mix_Init(0)) { // Đóng tất cả các sub-system đã init
         Mix_Quit();
    }
    std::cout << "SDL_mixer cleaned up." << std::endl;
}

bool SoundManager::loadSoundEffect(const std::string& id, const std::string& filePath) {
    if (!initialized_) return false;

     if (soundEffects_.count(id) && soundEffects_[id] != nullptr) {
        Mix_FreeChunk(soundEffects_[id]);
    }

    Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
    if (!chunk) {
        std::cerr << "Failed to load sound effect: " << filePath << "! Mix_Error: " << Mix_GetError() << std::endl;
        soundEffects_[id] = nullptr;
        return false;
    }
    soundEffects_[id] = chunk;
    return true;
}

void SoundManager::playSoundEffect(const std::string& id, int loops) {
    if (!initialized_) return;

    if (soundEffects_.count(id) && soundEffects_[id] != nullptr) {
        int channel = Mix_PlayChannel(-1, soundEffects_[id], loops);
        if (channel == -1) {
            std::cerr << "Failed to play sound effect: " << id << "! Mix_Error: " << Mix_GetError() << std::endl;
        }
    } else {
        std::cerr << "Sound effect not found or failed to load: " << id << std::endl;
    }
}

bool SoundManager::loadMusic(const std::string& filePath) {
    if (!initialized_) return false;

    if (backgroundMusic_) {
        Mix_HaltMusic();
        Mix_FreeMusic(backgroundMusic_);
        backgroundMusic_ = nullptr;
    }

    backgroundMusic_ = Mix_LoadMUS(filePath.c_str());
    if (!backgroundMusic_) {
        std::cerr << "Failed to load music: " << filePath << "! Mix_Error: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

// Play Music
void SoundManager::playMusic(int loops) {
    if (!initialized_ || !backgroundMusic_) {
         if (!backgroundMusic_) std::cerr << "No music loaded to play." << std::endl;
         return;
     }

    // Phát nhạc nền
    if (Mix_PlayMusic(backgroundMusic_, loops) == -1) {
        std::cerr << "Failed to play music! Mix_Error: " << Mix_GetError() << std::endl;
    }
}

// Stop Music
void SoundManager::stopMusic() {
    if (!initialized_) return;
    Mix_HaltMusic(); // Dừng phát nhạc
}

// Pause Music
void SoundManager::pauseMusic() {
    if (!initialized_) return;
    Mix_PauseMusic();
}

// Resume Music
void SoundManager::resumeMusic() {
    if (!initialized_) return;
    Mix_ResumeMusic();
}

// Set Sound Effects Volume
void SoundManager::setSoundEffectsVolume(int volume) {
     if (!initialized_) return;
     if (volume < 0) volume = 0;
    if (volume > MIX_MAX_VOLUME) volume = MIX_MAX_VOLUME;
    // Đặt âm lượng cho tất cả các kênh (thay vì từng chunk riêng lẻ)
     Mix_Volume(-1, volume); // -1 áp dụng cho tất cả các kênh
     std::cout << "Sound effects volume set to " << volume << std::endl;

}

// Set Music Volume
void SoundManager::setMusicVolume(int volume) {
    if (!initialized_) return;
     if (volume < 0) volume = 0;
    if (volume > MIX_MAX_VOLUME) volume = MIX_MAX_VOLUME;
    Mix_VolumeMusic(volume);
    std::cout << "Music volume set to " << volume << std::endl;
}

// Is Initialized
bool SoundManager::isInitialized() const {
    return initialized_;
}
