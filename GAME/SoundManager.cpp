
#include "SoundManager.h"

SoundManager::SoundManager() {
    int flags = MIX_INIT_MP3 | MIX_INIT_OGG;
    int initted = Mix_Init(flags);
    if ((initted & flags) != flags) {
        std::cerr << "Mix_Init: Failed to init required ogg and mp3 support!\n";
        std::cerr << "Mix_Init: " << Mix_GetError() << std::endl;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! Mix_Error: " << Mix_GetError() << std::endl;
        initialized_ = false;
    } else {
        initialized_ = true;
        std::cout << "SDL_mixer initialized successfully!" << std::endl;
        setSoundEffectsVolume(MIX_MAX_VOLUME / 2); // 50% volume
        setMusicVolume(MIX_MAX_VOLUME / 2);
    }
}

SoundManager::~SoundManager() {
    for (auto const& [id, chunk] : soundEffects_) {
        if (chunk) {
            Mix_FreeChunk(chunk);
        }
    }
    soundEffects_.clear();

    if (backgroundMusic_) {
        Mix_FreeMusic(backgroundMusic_);
        backgroundMusic_ = nullptr;
    }

    Mix_CloseAudio();

    while (Mix_Init(0)) {
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
    std::cout <<"render duoc roi\n";
    return true;
}

void SoundManager::playMusic(int loops) {
    if (!initialized_ || !backgroundMusic_) {
         if (!backgroundMusic_) std::cerr << "No music loaded to play." << std::endl;
         return;
     }

    if (Mix_PlayMusic(backgroundMusic_, loops) == -1) {
        std::cerr << "Failed to play music! Mix_Error: " << Mix_GetError() << std::endl;
    }
}

void SoundManager::stopMusic() {
    if (!initialized_) return;
    Mix_HaltMusic(); // Dừng phát nhạc
}

void SoundManager::pauseMusic() {
    if (!initialized_) return;
    Mix_PauseMusic();
}

void SoundManager::resumeMusic() {
    if (!initialized_) return;
    Mix_ResumeMusic();
}

void SoundManager::setSoundEffectsVolume(int volume) {
     if (!initialized_) return;
     if (volume < 0) volume = 0;
    if (volume > MIX_MAX_VOLUME) volume = MIX_MAX_VOLUME;
     Mix_Volume(-1, volume);
     std::cout << "Sound effects volume set to " << volume << std::endl;

}

void SoundManager::setMusicVolume(int volume) {
    if (!initialized_) return;
     if (volume < 0) volume = 0;
    if (volume > MIX_MAX_VOLUME) volume = MIX_MAX_VOLUME;
    Mix_VolumeMusic(volume);
    std::cout << "Music volume set to " << volume << std::endl;
}

bool SoundManager::isInitialized() const {
    return initialized_;
}
