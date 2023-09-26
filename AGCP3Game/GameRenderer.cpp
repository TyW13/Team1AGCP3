//
// Game.cpp
//
#include "GameRenderer.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

GameRenderer::GameRenderer(DeviceManager* _dManager) noexcept(false)
    : dManager(_dManager)
{
    
}

GameRenderer::~GameRenderer()
{
    if (dManager->GetDeviceResources())
    {
        dManager->GetDeviceResources()->WaitForGpu();
    }
}

// Initialize the Direct3D resources required to run.
void GameRenderer::Initialize(HWND window, int width, int height)
{
    framework.Init(dManager);
}

#pragma region Frame Update
// Executes the basic game loop.
void GameRenderer::Tick()
{
    m_timer.Tick([&]()
        {
            Update(m_timer);
        });

    Render();
}

// Updates the world.
void GameRenderer::Update(DX::StepTimer const& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    float elapsedTime = float(timer.GetElapsedSeconds());
   
    // Game logic
    framework.Update(dManager, elapsedTime);

    PIXEndEvent();
}
#pragma endregion
/////
#pragma region Frame Render
// Draws the scene.
void GameRenderer::Render()
{
    dManager->BeginRender();

    dManager->GetSpriteBatch()->Begin(dManager->GetCommandList());

    framework.Render(dManager);

    dManager->GetSpriteBatch()->End();

    dManager->EndRender();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void GameRenderer::OnActivated()
{

}

void GameRenderer::OnDeactivated()
{

}

void GameRenderer::OnSuspending()
{
    framework.OnSuspending();
}

void GameRenderer::OnResuming()
{
    m_timer.ResetElapsedTime();

    framework.OnResuming();
}

void GameRenderer::OnWindowMoved()
{
    auto const r = dManager->GetDeviceResources()->GetOutputSize();
    dManager->GetDeviceResources()->WindowSizeChanged(r.right, r.bottom);
}

void GameRenderer::OnDisplayChange()
{
    dManager->GetDeviceResources()->UpdateColorSpace();
}

void GameRenderer::OnWindowSizeChanged(int width, int height)
{
    if (!dManager->GetDeviceResources()->WindowSizeChanged(width, height))
        return;

    dManager->CreateWindowSizeDependentResources();
}

// Properties
void GameRenderer::GetDefaultSize(int& width, int& height) const noexcept
{
    width = 1920;
    height = 1080;
}
#pragma endregion