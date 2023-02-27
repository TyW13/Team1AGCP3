#include "Texture.h"
#include <filesystem>

// Texture.cpp

using namespace std;


Texture::Texture(ID3D12Device* m_device, const wchar_t* filename)
    : m_pGraphicsDevice(m_device)
{
    HRESULT hr;

    // Determine file type based on file extension
    std::wstring ext = std::filesystem::path(filename).extension().wstring();
    if (ext == L".dds")
    {
        // Load DDS texture using DirectXTex
        hr = DirectX::CreateDDSTextureFromFile(
            m_pGraphicsDevice->GetDevice(),
            filename,
            &m_pTexture,
            &m_SrvDesc);
    }
    else
    {
        // Load non-DDS texture using WIC (Windows Imaging Component)
        hr = DirectX::CreateWICTextureFromFile(
            m_pGraphicsDevice->GetDevice(),
            filename,
            &m_pTexture,
            &m_SrvDesc);
    }

    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to load texture");
    }
}

Texture::~Texture()
{
    if (m_pTexture != nullptr)
    {
        m_pTexture->Release();
        m_pTexture = nullptr;
    }
}


//std::unique_ptr<Texture> texture;
//texture = std::make_unique<Texture>(m_device.Get(), L"my_texture.png");

//std::unique_ptr<Texture> spriteTexture;
//spriteTexture = std::make_unique<Texture>(m_device.Get(), L"sprite.dds");