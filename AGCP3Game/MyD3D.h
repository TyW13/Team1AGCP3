//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "SpriteBatch.h"
#include <SpriteFont.h>

enum Descriptors;

// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class NewD3D final : public DX::IDeviceNotify
{
public:

    NewD3D() noexcept(false);
    ~NewD3D();

    NewD3D(NewD3D&&) = default;
    NewD3D& operator= (NewD3D&&) = default;

    NewD3D(NewD3D const&) = delete;
    NewD3D& operator= (NewD3D const&) = delete;

    // Initialization and management
    void Init(HWND window, int width, int height);

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    void BeginRender();
    void EndRender();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    enum Descriptors
    {
        Cat,
        Tile,
        Background,
        Count
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
    D3D12_GPU_DESCRIPTOR_HANDLE* GetSampler() { 
        assert(sampler);
        return sampler; }
    DirectX::DescriptorHeap* GetResourceDescriptors() { 
        assert(resourceDescriptors);
        return resourceDescriptors.get(); }
    Descriptors GetDescriptors() { 
        assert(mDescriptors);
        return mDescriptors; }

    //enum  Descriptors GetDescriptors() { return Descriptors; }
private:
    std::unique_ptr<DX::DeviceResources> deviceResources;
    ID3D12Device* device;
    std::unique_ptr<DirectX::ResourceUploadBatch> resourceUpload;
    std::unique_ptr<DirectX::DescriptorHeap> resourceDescriptors;
    ID3D12GraphicsCommandList* commandList;
    D3D12_GPU_DESCRIPTOR_HANDLE* sampler;

    void Clear();

    // Device resources.

    // Rendering loop timer.
    DX::StepTimer                               m_timer;

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;

    using VertexType = DirectX::VertexPositionColor;

    std::unique_ptr<DirectX::BasicEffect> m_effect;
    std::unique_ptr<DirectX::PrimitiveBatch<VertexType>> m_batch;

    DirectX::SimpleMath::Vector2 m_screenPos;
    DirectX::SimpleMath::Vector2 m_origin;

    std::unique_ptr<DirectX::CommonStates> m_states;

    RECT m_stretchRect;

    RECT m_fullscreenRect;


    Descriptors mDescriptors;
};