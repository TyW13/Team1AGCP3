#pragma once
#include "pch.h"
#include "DeviceResources.h"

class d3d : public DX::IDeviceNotify
{
private:
    std::unique_ptr<DX::DeviceResources> m_deviceResources;
    ID3D12Device* device;
    std::unique_ptr<DirectX::ResourceUploadBatch> resourceUpload;
    std::unique_ptr<DirectX::DescriptorHeap> resourceDescriptors;

public:
    d3d() {
    }

    void Init();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    DX::DeviceResources* GetDeviceResources() { return m_deviceResources.get(); }
    ID3D12Device* GetDevice() { return device; }
    DirectX::ResourceUploadBatch* GetResourceUpload() { return resourceUpload.get(); }
};

