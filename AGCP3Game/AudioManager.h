#pragma once

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

    void PlayShotgun();
    void PlayJump();
    void Playfootstep(float pitchMultiplier);
    void Stopfootstep();

    void OnNewAudioDevice() noexcept { m_retryAudio = true; }

  
private:

    std::unique_ptr<DirectX::AudioEngine> m_audEngine;

    std::unique_ptr<DirectX::SoundEffect> m_shotgun;
    std::unique_ptr<DirectX::SoundEffect> m_jump;
    std::unique_ptr<DirectX::SoundEffect> m_footstep;
    std::unique_ptr<DirectX::SoundEffectInstance> m_footstep_looped;

    bool m_retryAudio;

};

