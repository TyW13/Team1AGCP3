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

	//m_footsteploop.reset();
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
	auto m_footstep = std::make_unique<SoundEffect>(m_audEngine.get(),
		L"Data/footstep.wav");

	//m_footsteploop = m_footstep->CreateInstance();

	/*nightVolume = 1.f;
	nightSlide = -0.1f;*/
}

void AudioManager::Update(float timer)
{

	//if (m_retryAudio)
	//{
	//	m_retryAudio = false;
	//	if (m_audEngine->Reset())
	//	{
	//		// TODO: restart any looped sounds here

	//		// TODO: restart any looped sounds here
	//		/*if (m_nightLoop)
	//			m_nightLoop->Play(true);*/
	//	}
	//}
	//else if (!m_audEngine->Update())
	//{
	//	if (m_audEngine->IsCriticalError())
	//	{
	//		m_retryAudio = true;
	//	}
	//}
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
	//m_shotgun->Play();
}

void AudioManager::PlayJump()
{
	//m_jump->Play();
}

void AudioManager::Playfootstep()
{
	//m_footsteploop->Play(true);
}
