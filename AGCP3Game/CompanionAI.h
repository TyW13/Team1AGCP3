#pragma once

#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>

#include "DeviceResources.h"
#include "pch.h"

#include "d2d1.h"



class CompanionAI
{
public:
    CompanionAI(ID3D12Device* device);

    void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
    void Update(float deltaTime, const DirectX::XMFLOAT3& playerPosition);
    void Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* cbvHeap, UINT cbvOffset);

    void Speak(const wchar_t* message);

private:
    struct ConstantBuffer
    {
	    DirectX::XMFLOAT4X4 worldMatrix;
    };

    struct Vertex
    {
	    DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT2 uv;
    };

    struct SpeechBubbleVertex
    {
	    DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 uv;
    };

    struct SpeechBubbleConstantBuffer
    {
	    DirectX::XMFLOAT4X4 worldMatrix;
	    DirectX::XMFLOAT4 textColor;
    };

    struct companionAIConstantBuffer
    {
        DirectX::XMFLOAT4X4 worldMatrix;
        DirectX::XMFLOAT4 textColor;
    };
    std::unique_ptr<DX::DeviceResources>        m_deviceResources;

    Microsoft::WRL::ComPtr<ID3D12Device> m_device;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   m_commandList;

    Microsoft::WRL::ComPtr<ID3D12Resource> m_constantBuffer;
    ConstantBuffer* m_mappedConstantBuffer;

    Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_indexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
    UINT m_indexCount;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;

    // Companion Variables

    Microsoft::WRL::ComPtr<ID3D12Resource> m_companionAIVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_companionAIIndexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_companionAIVertexBufferView;
    D3D12_INDEX_BUFFER_VIEW m_companionAIIndexBufferView;
    UINT m_companionAIIndexCount;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_companionAIPipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_companionAIRootSignature;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_companionAIConstantBuffer;
    companionAIConstantBuffer* m_mappedcompanionAIConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_companionAITexture;

    // Shared Variables

	
    DirectX::XMFLOAT3 m_position;
    DirectX::XMFLOAT3 m_velocity;
    float m_moveSpeed = 2;
    float m_rotation;
    float m_scale;

    // Speech bubble variables
    Microsoft::WRL::ComPtr<ID3D12Resource> m_speechBubbleVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_speechBubbleIndexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_speechBubbleVertexBufferView;
    D3D12_INDEX_BUFFER_VIEW m_speechBubbleIndexBufferView;
    UINT m_speechBubbleIndexCount;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_speechBubblePipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_speechBubbleRootSignature;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_speechBubbleConstantBuffer;
    SpeechBubbleConstantBuffer* m_mappedSpeechBubbleConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_speechBubbleTexture;
    DirectX::XMFLOAT3 m_speechBubbleOffset;
  
};



//// Define the SpeechBubble class
//class SpeechBubble
//{
//
//public:
//
//    void Initialize(ID3D12Device* device, const wchar_t* message)
//    {
//        
//
//
//
//        // Set the text for the speech bubble
//        m_message = message;
//    }
//
//    void Render(ID3D12Device* device)
//    {
//        // Draw the speech bubble
//
//
//        // Draw the text
//
//    }
//
//    void SetPosition(D2D1_POINT_2F position)
//    {
//        m_position = position;
//    }
//
//
//
//};







//using Microsoft::WRL::ComPtr;
//using namespace DirectX;
//
//class CompanionAI
//{
//public :
//
//	CompanionAI(ID3D12Device* device);
//
//
//	void Update(float deltatime, const DirectX::XMFLOAT3& playerPosition);
//	void Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* cbvHeap, UINT cbvOffset);
//
//private:
//
//	struct ConstantBuffer
//	{
//		DirectX::XMFLOAT4X4 worldMatrix;
//
//	};
//
//
//
//    ComPtr<ID3D12Resource> m_constantBuffer;
//    ConstantBuffer* m_mappedConstantBuffer;
//    ComPtr<ID3D12PipelineState> m_pipelineState;
//    ComPtr<ID3D12RootSignature> m_rootSignature;
//    ComPtr<ID3D12Resource> m_vertexBuffer;
//    ComPtr<ID3D12Resource> m_indexBuffer;
//    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
//    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
//    UINT m_indexCount;
//    DirectX::XMFLOAT3 m_position;
//    DirectX::XMFLOAT3 m_velocity;
//    float m_rotation;
//    float m_scale;
//};

