#include "d3d.h"

void d3d::Init()
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);

    m_deviceResources->CreateDeviceResources();

    device = m_deviceResources->GetD3DDevice();
    resourceUpload = std::make_unique<DirectX::ResourceUploadBatch>(device);
}

void d3d::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    // If using the DirectX Tool Kit for DX12, uncomment this line:

    resourceDescriptors.reset();
}

void d3d::OnDeviceRestored()
{
}