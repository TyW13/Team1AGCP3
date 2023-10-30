#include "ContinueButton.h"
#include "ResourceManager.h"

void ContinueButton::Init(DeviceManager* dManager, std::string texPath, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, DirectX::SimpleMath::Vector2 size, RECT rect)
{
	mPos = position;
	mScale = scale;
	mSize = size;
	mRect = rect;

	mCollisionBounds.left = mPos.x;
	mCollisionBounds.top = mPos.y;
	mCollisionBounds.right = mPos.x + size.x * mScale.x;
	mCollisionBounds.bottom = mPos.y + size.y * mScale.y;

	std::wstring wTexPath(texPath.begin(), texPath.end());
	dManager->GetResourceUpload()->Begin();

	DX::ThrowIfFailed(
		DirectX::CreateDDSTextureFromFile(dManager->GetDevice(), *dManager->GetResourceUpload(), wTexPath.c_str(),
			texture.ReleaseAndGetAddressOf()));

	DirectX::CreateShaderResourceView(dManager->GetDevice(), texture.Get(),
		dManager->GetResourceDescriptors()->GetCpuHandle(3));

	auto uploadResourcesFinished = dManager->GetResourceUpload()->End(
		dManager->GetDeviceResources()->GetCommandQueue());
	uploadResourcesFinished.wait();
}

void ContinueButton::Render(DeviceManager* dManager)
{
	dManager->GetSpriteBatch()->Draw(dManager->GetResourceDescriptors()->GetGpuHandle(3),
		DirectX::GetTextureSize(texture.Get()),
		mPos, &mRect, { 1.f, 1.f, 1.f, 1.f }, 0.f, mOrigin, mScale);
}

void ContinueButton::OnClicked(ResourceManager* rManager, DeviceManager* dManager)
{
	rManager->LoadPlayerData();
	rManager->LoadGame(dManager);
}