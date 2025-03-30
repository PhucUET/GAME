#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SDL_mixer.h>
#include <string>
#include <map>
#include <iostream>

class SoundManager {
public:
    SoundManager();

    ~SoundManager();

    bool loadSoundEffect(const std::string& id, const std::string& filePath);

    void playSoundEffect(const std::string& id, int loops = 0);

    bool loadMusic(const std::string& filePath);

    void playMusic(int loops = -1);

    void stopMusic();

    void pauseMusic();

    void resumeMusic();

    void setSoundEffectsVolume(int volume);

    void setMusicVolume(int volume);

    bool isInitialized() const;

private:
    std::map<std::string, Mix_Chunk*> soundEffects_;

    Mix_Music* backgroundMusic_ = nullptr;

    bool initialized_ = false;
};

#endif // SOUNDMANAGER_H
