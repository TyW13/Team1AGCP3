
#include "Framework.h"
#include "SpriteBatch.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


void Framework::Initialise(HWND window, int width, int height)
{
    audio.Init();

    //game logic, will move later
    m_keyboard = std::make_unique<Keyboard>();
    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(window);
}
void Framework::Update(float elapsedTime)
{
    audio.Update(elapsedTime);

    //get keyboard stated
    auto kb = m_keyboard->GetState();
    auto mouse = m_mouse->GetState();

    player.Update(elapsedTime, kb, mouse);
}

void Framework::Terminate()
{
	
}

void Framework::OnSuspending()
{
    audio.OnSuspending();
}

void Framework::OnResuming()
{
    audio.OnResuming();
}
