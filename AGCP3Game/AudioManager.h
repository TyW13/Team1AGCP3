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
    void Playfootstep(float pitchMultiplier);   //pitchMultiplier for setting how quick the playback should be
    void Stopfootstep();

    void OnNewAudioDevice() noexcept { m_retryAudio = true; }

  
private:

    std::unique_ptr<DirectX::AudioEngine> m_audEngine;                      //audio engine

    std::unique_ptr<DirectX::SoundEffect> m_shotgun;                        //shotgun sound
    std::unique_ptr<DirectX::SoundEffect> m_jump;                           //jump sound
    std::unique_ptr<DirectX::SoundEffect> m_footstep;                       //footstep sound
    std::unique_ptr<DirectX::SoundEffectInstance> m_footstep_looped;        //footstep sound, for looping

    bool m_retryAudio;

};

