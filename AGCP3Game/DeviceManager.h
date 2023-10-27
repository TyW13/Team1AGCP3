#pragma once

#include "pch.h"
#include "DeviceResources.h"
#include "StepTimer.h"
#include <math.h>
#include <sstream>
#include <Keyboard.h>
#include <Mouse.h>

#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugStringA( os_.str().c_str() );  \
}                             \

enum Descriptors;

// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class DeviceManager final : public DX::IDeviceNotify
{
public:

    DeviceManager() noexcept(false);
    ~DeviceManager();

    DeviceManager(DeviceManager&&) = default;
    DeviceManager& operator= (DeviceManager&&) = default;

    DeviceManager(DeviceManager const&) = delete;
    DeviceManager& operator= (DeviceManager const&) = delete;

    // Initialization and management
    void Init(HWND window, int width, int height);

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    void BeginRender();
    void EndRender();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    DirectX::Keyboard* GetKeyboard() { return m_keyboard.get(); }
    DirectX::Mouse* GetMouse() { return m_mouse.get(); }

    enum Descriptors
    {
        Player,
        Tile,
        Shotgun,
        TitleImage,
        Count,
    };

    // Properties
    DX::DeviceResources* GetDeviceResources() { 
        assert(deviceResources);
        return deviceResources.get(); }
    ID3D12Device* GetDevice() { 
        assert(device);
        return device; }
    DirectX::ResourceUploadBatch* GetResourceUpload() { 
        assert(resourceUpload);
        return resourceUpload.get(); }
    ID3D12GraphicsCommandList* GetCommandList() { 
        assert(commandList);
        return commandList; }
    DirectX::DescriptorHeap* GetResourceDescriptors() { 
        assert(resourceDescriptors);
        return resourceDescriptors.get(); }
    Descriptors GetDescriptors() { 
        assert(mDescriptors);
        return mDescriptors; }
    DirectX::SpriteBatch* GetSpriteBatch() {
        assert(spriteBatch);
        return spriteBatch.get();
    }
    DirectX::SimpleMath::Vector2 GetPosition() {
        return m_screenPos;
    }
    DirectX::SimpleMath::Vector2 GetOrigin() {
        return m_origin;
    }
    RECT GetFullScreenRect() {
        return m_fullscreenRect;
    }

private:
    std::unique_ptr<DX::DeviceResources> deviceResources;
    ID3D12Device* device;
    std::unique_ptr<DirectX::ResourceUploadBatch> resourceUpload;
    std::unique_ptr<DirectX::DescriptorHeap> resourceDescriptors;
    ID3D12GraphicsCommandList* commandList;

    void Clear();

    // Device resources.

    // Rendering loop timer.
    DX::StepTimer                               m_timer;

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;

    using VertexType = DirectX::VertexPositionColor;

    std::unique_ptr<DirectX::BasicEffect> m_effect;
    std::unique_ptr<DirectX::PrimitiveBatch<VertexType>> m_batch;

    std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
    DirectX::SimpleMath::Vector2 m_screenPos;
    DirectX::SimpleMath::Vector2 m_origin;

    std::unique_ptr<DirectX::CommonStates> m_states;

    RECT m_tileRect;
    RECT m_stretchRect;
    RECT m_fullscreenRect;


    Descriptors mDescriptors;

    std::unique_ptr<DirectX::Keyboard> m_keyboard;
    std::unique_ptr<DirectX::Mouse> m_mouse;
};