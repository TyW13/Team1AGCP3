//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "SpriteBatch.h"
#include <SpriteFont.h>

class Framework;
class Input;
// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class GameRenderer final : public DX::IDeviceNotify
{
public:

    GameRenderer() noexcept(false);
    ~GameRenderer();

    GameRenderer(GameRenderer&&) = default;
    GameRenderer& operator= (GameRenderer&&) = default;

    GameRenderer(GameRenderer const&) = delete;
    GameRenderer& operator= (GameRenderer const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

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
    Framework* m_pFramework;

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>        m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                               m_timer;

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;

    using VertexType = DirectX::VertexPositionColor;

    std::unique_ptr<DirectX::BasicEffect> m_effect;
    std::unique_ptr<DirectX::PrimitiveBatch<VertexType>> m_batch;

    std::unique_ptr<DirectX::DescriptorHeap> m_resourceDescriptors;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;

    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    DirectX::SimpleMath::Vector2 m_screenPos;
    DirectX::SimpleMath::Vector2 m_origin;

    RECT m_tileRect;
    std::unique_ptr<DirectX::CommonStates> m_states;

    RECT m_stretchRect;

    RECT m_fullscreenRect;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_background;

    enum Descriptors
    {
        Cat,
        Background,
        Count
    };
};

