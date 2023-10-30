#include "TitleImage.h"

void TitleImage::Init(DeviceManager* dManager, std::string texPath, DirectX::SimpleMath::Vector2 _position, DirectX::SimpleMath::Vector2 _scale)
{
	mPos = _position;
	mScale = _scale;

	std::wstring wTexPath(texPath.begin(), texPath.end());
	dManager->GetResourceUpload()->Begin();

	DX::ThrowIfFailed(
		DirectX::CreateDDSTextureFromFile(dManager->GetDevice(), *dManager->GetResourceUpload(), wTexPath.c_str(),
			objTex.ReleaseAndGetAddressOf()));

	DirectX::CreateShaderResourceView(dManager->GetDevice(), objTex.Get(),
		dManager->GetResourceDescriptors()->GetCpuHandle(3));

	auto uploadResourcesFinished = dManager->GetResourceUpload()->End(
		dManager->GetDeviceResources()->GetCommandQueue());
	uploadResourcesFinished.wait();
}

void TitleImage::Render(DeviceManager* dManager)
{
	RECT imgRect;
	imgRect.left = 0;
	imgRect.top = 0;
	imgRect.right = 1920;
	imgRect.bottom = 1080;

	RECT* sourceRect = &imgRect;


	dManager->GetSpriteBatch()->Draw(dManager->GetResourceDescriptors()->GetGpuHandle(3),
		DirectX::GetTextureSize(objTex.Get()),
		mPos);


}
