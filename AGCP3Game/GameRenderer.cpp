//
// Game.cpp
//
#include "stdafx.h"

#include "GameRenderer.h"
#include "Framework.h"

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
    audio.Init();
    rManager.Init(dManager);
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

    rManager.Update(elapsedTime);
    audio.Update(elapsedTime);

    /// /////////////////////////////////////////////////////////////////////
    /// /////////////////////////////////////////////////////////////////////
    /// This is our game logic all here!
    /// /////////////////////////////////////////////////////////////////////
    /// /////////////////////////////////////////////////////////////////////
    /// /////////////////////////////////////////////////////////////////////
    /// /////////////////////////////////////////////////////////////////////
    /// /////////////////////////////////////////////////////////////////////


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


    rManager.Render(dManager);


    dManager->GetSpriteBatch()->End();

    dManager->EndRender();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void GameRenderer::OnActivated()
{
    // TODO: Game is becoming active window.
}

void GameRenderer::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void GameRenderer::OnSuspending()
{
    audio.OnSuspending();

    // TODO: Game is being power-suspended (or minimized).
}

void GameRenderer::OnResuming()
{
    m_timer.ResetElapsedTime();

    audio.OnResuming();

    // TODO: Game is being power-resumed (or returning from minimize).
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

    // TODO: Game window is being resized.
}

// Properties
void GameRenderer::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 1920;
    height = 1080;
}
#pragma endregion