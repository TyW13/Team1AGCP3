//
// Game.h
//

#pragma once

#include "MyD3D.h"
#include "StepTimer.h"
#include <SpriteFont.h>
#include "AudioManager.h"

class Framework;
// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class GameRenderer final
{
public:
    GameRenderer(NewD3D* _mD3D) noexcept(false);
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

    //DX::DeviceResources* GetDeviceResources() { return m_deviceResources.get(); }
    //ID3D12Device* GetDevice() { return device; }
    //ResourceUploadBatch* GetResourceUpload() { return resourceUpload.get(); }
    
private:
    DirectX::SpriteFont* mpSF = nullptr;

    void Update(DX::StepTimer const& timer);
    void Render();

    void CreateDeviceDependentResources();

    // Device resources.
    NewD3D* mD3D;

    //std::unique_ptr<DX::DeviceResources> m_deviceResources;
    //ID3D12Device* device;
    //std::unique_ptr <ResourceUploadBatch> resourceUpload;

    // Rendering loop timer.
    DX::StepTimer m_timer;

    // If using the DirectX Tool Kit for DX12, uncomment this line:

    using VertexType = DirectX::VertexPositionColor;


    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    DirectX::SimpleMath::Vector2 m_screenPos;
    DirectX::SimpleMath::Vector2 m_origin;

    RECT m_tileRect;
    RECT m_stretchRect;
    RECT m_fullscreenRect;

    AudioManager audio;

    enum Descriptors
    {
        Cat,
        Tile,
        Background,
        Count
    };
};