#pragma once
#include <d3d12.h>
#include <stdexcept>
#include <string>
#include <WICTextureLoader.h>
#include "DirectXTex/DirectXTex.h"
#include  "Renderer.h"
#include "d3dx12.h"
#include "DDSTextureLoader.h"


class Texture
{
    Texture(ID3D12Device* device, LPCWSTR filename)
        : m_device(device)
        , m_textureResource(nullptr)
        , m_textureUploadHeap(nullptr)
        , m_textureDesc({})
        , m_textureSize(0)
    {
        // Load Texture from DDS file
      
        DirectX::ScatchImage scatchImage;
        HRESULT hr = DirectX::LoadDDSTextureFromFile(filename, DirectX::DDS_FLAGS_NONE, nullptr, scratchImage);
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to load texture from DDS file.");
        }

        // Get texture description and size
        const DirectX::TexMetadata& texMetadata = scratchImage.GetMetadata();
        m_textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(texMetadata.format, texMetadata.width, texMetadata.height, (WORD)texMetadata.arraySize, (WORD)texMetadata.mipLevels);


        // Create texture resource in the default heap
        hr = m_device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &m_textureDesc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(&m_textureResource));

        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to create texture resource.");
        }
        m_textureResource->SetName(L"Texture Resource");

        // Create upload heap to copy texture data to GPU
        UINT64 uploadBufferSize;
        m_device->GetCopyableFootprints(&m_textureDesc, 0, (UINT)texMetadata.mipLevels, 0, nullptr, nullptr, nullptr, &uploadBufferSize);
        hr = m_device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&m_textureUploadHeap));
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to create texture upload heap.");
        }
        m_textureUploadHeap->SetName(L"Texture Upload Heap");

        // Copy texture data to upload heap
        D3D12_SUBRESOURCE_DATA textureData = {};
        textureData.pData = scratchImage.GetPixels();
        textureData.RowPitch = scratchImage.GetMetadata().rowPitch;
        textureData.SlicePitch = scratchImage.GetMetadata().depthPitch;
        UpdateSubresources(m_commandList.Get(), m_textureResource.Get(), m_textureUploadHeap.Get(), 0, 0, (UINT)texMetadata.mipLevels, &textureData);

        // Create SRV descriptor heap and descriptor for texture
        D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
        srvHeapDesc.NumDescriptors = 1;
        srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        hr = m_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvDescriptorHeap));
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to create SRV descriptor heap.");
        }

        CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(m_srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
        m_textureSize = static_cast<UINT>(scratchImage.GetPixelsSize());
        m_device->CreateShaderResourceView(m_textureResource.Get(), nullptr, srvHandle);
    }
    
    // Destructor
    ~Texture()
    {
        // Release resources
        m_textureResource.Reset();
        m_textureUploadHeap.Reset();
        m_srvDescriptorHeap.Reset();
    }

    // Getters
    ID3D12Resource* GetResource() const { return m_textureResource.Get(); }
    ID3D12DescriptorHeap* GetSRVDescriptorHeap() const { return m_srvDescriptorHeap.Get(); }
    UINT GetTextureSize() const { return m_textureSize; }


private:
    // Texture resources
    ID3D12Device* m_device;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_textureResource;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_textureUploadHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_srvDescriptorHeap;

    // Texture description and size
    D3D12_RESOURCE_DESC m_textureDesc;
    UINT m_textureSize;
   
};

