#include "AudioManager.h"

AudioManager::AudioManager() noexcept(false) :
	m_retryAudio(false)
{

}

AudioManager::~AudioManager()
{
	if (m_audEngine)
	{
		m_audEngine->Suspend();
	}

	m_footstep_looped.reset();
}

void AudioManager::Init()
{
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags |= AudioEngine_Debug;
#endif
	m_audEngine = std::make_unique<AudioEngine>(eflags);

	m_shotgun = std::make_unique<SoundEffect>(m_audEngine.get(),
		L"Data/shotgun.wav");
	m_jump = std::make_unique<SoundEffect>(m_audEngine.get(),
		L"Data/jump.wav");
	m_footstep = std::make_unique<SoundEffect>(m_audEngine.get(),
		L"Data/footstep.wav");

	// Create an instance of the sound effect and set it to loop
	m_footstep_looped = m_footstep->CreateInstance();
}

void AudioManager::Update(float timer)
{
	if (m_retryAudio)
	{
		m_retryAudio = false;

		if (m_audEngine->Reset())
		{
			// TODO: restart any looped sounds here
			if (m_footstep_looped)
				m_footstep_looped->Play(true);
		}
	}

}

void AudioManager::Render()
{
}

void AudioManager::OnSuspending()
{
	m_audEngine->Suspend();
}

void AudioManager::OnResuming()
{

	m_audEngine->Resume();
}

void AudioManager::PlayShotgun()
{
	if (m_shotgun)
	{
		m_shotgun->Play();
	}
}

void AudioManager::PlayJump()
{
	if (m_jump)
	{
		m_jump->Play();
	}
}

void AudioManager::Playfootstep(float pitchMultiplier)
{
	if (m_footstep_looped)
	{
		m_footstep_looped->SetPitch(pitchMultiplier);
		m_footstep_looped->Play(true);
	}
}

void AudioManager::Stopfootstep()
{
	if (m_footstep_looped->IsLooped())
	{
		m_footstep_looped->Stop();
	}
}