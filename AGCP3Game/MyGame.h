#pragma once

#ifndef MYGAME_H
#define MYGAME_H

#include <wrl/client.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>

class MyGame
{
public:
    MyGame();
    virtual ~MyGame();

    void Initialize(HWND hwnd);
    void CreateDevice();

    void CreateCommandQueue();
    void CreateCommandAllocator();
    void CreateCommandList();

    void LoadContent();

    void UpdatePlayer(float deltaTime);


  /*  void UpdateEnemies(float deltaTime);*/
    void Update(float deltaTime);


    void LoadTextures();

    void ClearViews();

    void RenderObjects();

    void RenderUI();
    
    void Present();

    void Render();

private:
    HWND m_hwnd;

    // DirectX 12 resources
    Microsoft::WRL::ComPtr<ID3D12Device> m_device;
    Microsoft::WRL::ComPtr<IDXGISwapChain4> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocators[2];
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[2];
    Microsoft::WRL::ComPtr<ID3D12Resource> m_depthStencilBuffer;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsvHeap;


    ID3D12CommandAllocator* m_commandAllocator;

    // Game objects and state
    DirectX::XMFLOAT3 m_playerPosition;
    DirectX::XMFLOAT3 m_playerVelocity;
    float m_playerRotation;
    float m_playerScale;

    // Game objects and state
    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 uv;
    };
    Microsoft::WRL::ComPtr<ID3D12Resource> m_spriteVertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_spriteVertexBufferView;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_spriteTexture;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_spriteDescriptorHeap;

    // Constant buffer data
    struct ConstantBufferData
    {
        DirectX::XMFLOAT4X4 transform;
    };
    Microsoft::WRL::ComPtr<ID3D12Resource> m_constantBuffer;
    ConstantBufferData m_constantBufferData;

    // View and projection matrices
    DirectX::XMFLOAT4X4 m_viewMatrix;
    DirectX::XMFLOAT4X4 m_projectionMatrix;


};

#endif // MYGAME_H