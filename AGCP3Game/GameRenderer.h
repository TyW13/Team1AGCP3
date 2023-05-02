//
// Game.h
//

#pragma once

#include "Framework.h"

class GameRenderer final
{
public:
    GameRenderer(DeviceManager* _dManager) noexcept(false);
    ~GameRenderer();

    GameRenderer(GameRenderer&&) = default;
    GameRenderer& operator= (GameRenderer&&) = default;

    GameRenderer(GameRenderer const&) = delete;
    GameRenderer& operator= (GameRenderer const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize(int& width, int& height) const noexcept;

private:
    DirectX::SpriteFont* mpSF = nullptr;

    void Update(DX::StepTimer const& timer);
    void Render();

    Framework framework;

    // Device resources.
    DeviceManager* dManager;

    // Rendering loop timer.
    DX::StepTimer m_timer;

    // If using the DirectX Tool Kit for DX12, uncomment this line:

    using VertexType = DirectX::VertexPositionColor;


    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
};