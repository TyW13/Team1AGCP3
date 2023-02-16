

#include "stdafx.h"
#include "DDSTextureLoader.h"
#include "SpriteClass.h"
#include <fstream>
#include "DDSTextureLoader.h"

Sprite::Sprite(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const wchar_t* fileName, int spriteWidth, int spriteHeight) :
    m_position(0.0f, 0.0f),
    m_velocity(0.0f, 0.0f),
    m_scale(1.0f),
    m_rotation(0.0f),
    m_device(device),
    m_loadingComplete(false)
{
    CreateDeviceDependentResources(fileName);
    CreateTextureResource(fileName);
    CreateVertexBuffer(spriteWidth, spriteHeight);

    m_commandList = commandList;
}

Sprite::~Sprite()
{
}

void Sprite::Update(float deltaTime)
{
    // Update sprite position based on velocity
    m_position.x += m_velocity.x * deltaTime;
    m_position.y += m_velocity.y * deltaTime;
}

void Sprite::Draw()
{

    // Set the vertex buffer and texture
    m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
    m_commandList->SetGraphicsRootDescriptorTable(0, m_srvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

    // Draw the sprite
    m_commandList->DrawInstanced(6, 1, 0, 0);
}


void Sprite::SetPosition(float x, float y)
{
    m_position = DirectX::XMFLOAT2(x, y);
}

void Sprite::Move(float dx, float dy)
{
    m_x += dx;
    m_y += dy;
}

void Sprite::SetVelocity(float x, float y)
{
    m_velocity = DirectX::XMFLOAT2(x, y);
}
//
void Sprite::SetScale(float scale)
{
    m_scale = scale;
}




void Sprite::CreateDeviceDependentResources(const wchar_t* fileName)
{

}

void Sprite::CreateTextureResource(const wchar_t* fileName)
{

}

void Sprite::CreateVertexBuffer(int spriteWidth, int spriteHeight)
{

}


//// Load the texture from a file
//bool Sprite::LoadTextureFromFile(ID3D12Device* device, const wchar_t* fileName)
//{
//    // Load the texture
//    HRESULT hr = CreateDDSTextureFromFile(device, fileName, &m_texture, &m_textureUploadHeap);
//    if (FAILED(hr))
//    {
//        return false;
//    }
//
//    // Get the texture description to obtain the width and height
//    D3D12_RESOURCE_DESC textureDesc = m_texture->GetDesc();
//    m_textureWidth = textureDesc.Width;
//    m_textureHeight = textureDesc.Height;
//
//    // Create the sampler for the texture
//    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
//    heapDesc.NumDescriptors = 1;
//    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
//    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
//    device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_samplerHeap));
//
//    D3D12_SAMPLER_DESC samplerDesc = {};
//    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
//    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//    samplerDesc.MinLOD = 0;
//    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
//    samplerDesc.MipLODBias = 0.0f;
//    samplerDesc.MaxAnisotropy = 1;
//    device->CreateSampler(&samplerDesc, m_samplerHeap->GetCPUDescriptorHandleForHeapStart());
//
//    return true;
//}
