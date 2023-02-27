#pragma once
#include <d3d12.h>
#include <stdexcept>
#include <string>
#include <WICTextureLoader.h>
#include  "Render.h"
#include "d3dx12.h"

class Texture
{
    Texture(ID3D12Device* device, const std::wstring& filename)
        : m_device(device)
        , m_textureResource(nullptr)
        , m_textureUploadHeap(nullptr)
        , m_textureDesc({})
        , m_textureSize(0)
    {
        LoadTexture(filename);
    }

    ~Texture()
    {
        ReleaseTexture();
    }

    ID3D12Resource* GetTextureResource() const
    {
        return m_textureResource.Get();
    }

    CD3DX12_CPU_DESCRIPTOR_HANDLE GetSRVHandle() const
    {
        return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    }

private:
    void LoadTexture(const std::wstring& filename)
    {
        // Load the texture from file using the WIC library
        std::unique_ptr<uint8_t[]> textureData;
        D3D12_RESOURCE_DESC textureDesc = {};
        UINT textureSize = 0;
        HRESULT hr = DirectX::LoadWICTextureFromFile(filename.c_str(), textureData, textureDesc, textureSize);
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to load texture from file.");
        }

        // Create the texture resource
        CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);
        hr = m_device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &textureDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(m_textureResource.ReleaseAndGetAddressOf()));
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to create texture resource.");
        }
        m_textureResource->SetName(L"Texture Resource");

        // Create the texture upload heap
        CD3DX12_HEAP_PROPERTIES uploadHeapProps(D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC uploadDesc = CD3DX12_RESOURCE_DESC::Buffer(textureSize);
        hr = m_device->CreateCommittedResource(&uploadHeapProps, D3D12_HEAP_FLAG_NONE, &uploadDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(m_textureUploadHeap.ReleaseAndGetAddressOf()));
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to create texture upload heap.");
        }
        m_textureUploadHeap->SetName(L"Texture Upload Heap");

        // Copy the texture data to the upload heap
        D3D12_SUBRESOURCE_DATA textureDataDesc = {};
        textureDataDesc.pData = textureData.get();
        textureDataDesc.RowPitch = textureDesc.Width * textureDesc.Format->BytesPerPixel;
        textureDataDesc.SlicePitch = textureDataDesc.RowPitch * textureDesc.Height;

        UpdateSubresources(m_commandList.Get(), m_textureResource.Get(), m_textureUploadHeap.Get(), 0, 0, 1, &textureDataDesc);

        // Transition the texture resource to the shader resource state
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_textureResource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
        m_commandList->ResourceBarrier(1, &barrier);

        // Create the SRV descriptor heap and descriptor for the texture
        CD3DX12_DESCRIPTOR_HEAP_DESC srvDescriptorHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1);
        hr = m_device->CreateDescriptorHeap(&srvDescriptorHeapDesc, IID_PPV_ARGS);

        (m_srvDescriptorHeap.ReleaseAndGetAddressOf()));
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to create SRV descriptor heap.");
        }
        m_srvDescriptorHeap->SetName(L"SRV Descriptor Heap");

        
            
    	CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(m_srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
        m_device->CreateShaderResourceView(m_textureResource.Get(), nullptr, srvHandle);

        // Store the texture description and size
        m_textureDesc = textureDesc;
        m_textureSize = textureSize;
    }

    void ReleaseTexture()
    {
        m_textureResource.Reset();
        m_textureUploadHeap.Reset();
        m_srvDescriptorHeap.Reset();
        m_textureDesc = {};
        m_textureSize = 0;
    }


    private:
        Microsoft::WRL::ComPtr<ID3D12Device> m_device;
        Microsoft::WRL::ComPtr<ID3D12Resource> m_textureResource;
        Microsoft::WRL::ComPtr<ID3D12Resource> m_textureUploadHeap;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_srvDescriptorHeap;
        D3D12_RESOURCE_DESC m_textureDesc;
        UINT m_textureSize;
};

