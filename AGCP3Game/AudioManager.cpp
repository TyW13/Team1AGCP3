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

	m_nightLoop.reset();
}

void AudioManager::Init()
{
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags |= AudioEngine_Debug;
#endif
	m_audEngine = std::make_unique<AudioEngine>(eflags);

	m_explode = std::make_unique<SoundEffect>(m_audEngine.get(),
		L"Data/angry cat meow.wav");
	m_ambient = std::make_unique<SoundEffect>(m_audEngine.get(),
		L"Data/la chanson du chat.wav");

	std::random_device rd;
	m_random = std::make_unique<std::mt19937>(rd());

	explodeDelay = 2.f;

	m_nightLoop = m_ambient->CreateInstance();
	m_nightLoop->Play(true);

	nightVolume = 1.f;
	nightSlide = -0.1f;
}

void AudioManager::Update(float timer)
{
	float elapsedTime = float(timer);

	nightVolume += elapsedTime * nightSlide;
	if (nightVolume < 0.2f)
	{
		nightVolume = 0.2f;
		nightSlide = -nightSlide;
	}
	else if (nightVolume > 1.f)
	{
		nightVolume = 1.f;
		nightSlide = -nightSlide;
	}
	m_nightLoop->SetVolume(nightVolume);

	explodeDelay -= elapsedTime;

	if (explodeDelay < 0.f)
	{
		m_explode->Play();

		std::uniform_real_distribution<float> dist(1.f, 10.f);
		explodeDelay = dist(*m_random);
	}

	if (m_retryAudio)
	{
		m_retryAudio = false;
		if (m_audEngine->Reset())
		{
			// TODO: restart any looped sounds here

			// TODO: restart any looped sounds here
			if (m_nightLoop)
				m_nightLoop->Play(true);
		}
	}
	else if (!m_audEngine->Update())
	{
		if (m_audEngine->IsCriticalError())
		{
			m_retryAudio = true;
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
	explodeDelay = 2.f;

	m_audEngine->Resume();
}
