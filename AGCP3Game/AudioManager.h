#pragma once

//#include "DeviceResources.h"

//class created with help of:
//https://github.com/microsoft/DirectXTK/wiki/Creating-and-playing-sounds

#include <Audio.h>
#include <random>

using namespace DirectX;

class AudioManager
{
public:

    AudioManager() noexcept(false);
    ~AudioManager();

    void Init();
    void Update(float timer);
    void Render();
    void OnSuspending();
    void OnResuming();

    void OnNewAudioDevice() noexcept { m_retryAudio = true; }

private:

    std::unique_ptr<std::mt19937> m_random;
    std::unique_ptr<DirectX::AudioEngine> m_audEngine;
    std::unique_ptr<DirectX::SoundEffect> m_explode;
    std::unique_ptr<DirectX::SoundEffect> m_ambient;
    std::unique_ptr<DirectX::SoundEffectInstance> m_nightLoop;

    bool m_retryAudio;

    float explodeDelay;
    float nightVolume;
    float nightSlide;

};

