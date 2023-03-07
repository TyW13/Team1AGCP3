

// Texture.cpp

#include <DirectXTex/DirectXTex.h>
using namespace std;


#include "Texture.h"
#include <wincodec.h>
#include <DirectXTex/DirectXTex.h>

#pragma comment(lib, "Windowscodecs.lib")

Texture::Texture(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const std::wstring& fileName)
{
    // Load the image file
    DirectX::TexMetadata metadata;
    DirectX::ScratchImage scratchImage;
    HRESULT hr = DirectX::LoadFromWICFile(fileName.c_str(), DirectX::WIC_FLAGS_NONE, &metadata, scratchImage);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to load texture from file.");
    }

    // Create the texture resource
    D3D12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(metadata.format, metadata.width, metadata.height, 1, 1);
    HRESULT result = device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&m_texture)
    );
    if (FAILED(result))
    {
        throw std::runtime_error("Failed to create texture resource.");
    }

    // Upload the texture data to the GPU
    Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer;
    heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(scratchImage.GetPixelsSize());
    result = device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&uploadBuffer)
    );
    if (FAILED(result))
    {
        throw std::runtime_error("Failed to create upload buffer for texture resource.");
    }

    D3D12_SUBRESOURCE_DATA subresourceData = {};
    subresourceData.pData = scratchImage.GetPixels();
    subresourceData.RowPitch = scratchImage.GetImages()->rowPitch;
    subresourceData.SlicePitch = scratchImage.GetPixelsSize();

    UpdateSubresources(commandList, m_texture.Get(), uploadBuffer.Get(), 0, 0, 1, &subresourceData);

    commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
        m_texture.Get(),
        D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE)
    );
}

Texture::~Texture()
{
}





//Texture::Texture(ID3D12Device* m_device, const wchar_t* filename)
//{
//    //HRESULT hr;
//
//    //// Determine file type based on file extension
//    //std::wstring ext = std::filesystem::path(filename).extension().wstring();
//    //if (ext == L".dds")
//    //{
//    //    // Load DDS texture using DirectXTex
//    //    hr = DirectX::CreateDDSTextureFromFile(
//    //        m_device->GetDevice(),
//    //        filename,
//    //        &m_pTexture,
//    //        &m_SrvDesc);
//    //}
//    //else
//    //{
//    //    // Load non-DDS texture using WIC (Windows Imaging Component)
//    //    hr = DirectX::CreateWICTextureFromFile(
//    //        m_device->GetDevice(),
//    //        filename,
//    //        &m_pTexture,
//    //        &m_SrvDesc);
//    //}
//
//    //if (FAILED(hr))
//    //{
//    //    throw std::runtime_error("Failed to load texture");
//    //}
//}
//
//Texture::~Texture()
//{
//    if (m_pTexture != nullptr)
//    {
//        m_pTexture->Release();
//        m_pTexture = nullptr;
//    }
//}


//std::unique_ptr<Texture> texture;
//texture = std::make_unique<Texture>(m_device.Get(), L"my_texture.png");

//std::unique_ptr<Texture> spriteTexture;
//spriteTexture = std::make_unique<Texture>(m_device.Get(), L"sprite.dds");