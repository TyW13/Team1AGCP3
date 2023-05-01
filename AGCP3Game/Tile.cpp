#include "Tile.h"

void Tile::Init(DeviceManager* dManager, std::wstring texPath, DirectX::SimpleMath::Vector2 _position, DirectX::SimpleMath::Vector2 _scale, bool _active, DirectX::SimpleMath::Vector2 _objSize, std::string _objType, int _collisionDirection, RECT _objRect)
{
	std::string tileRectsString = std::to_string(_objRect.left) + std::to_string(_objRect.top) + std::to_string(_objRect.right) + std::to_string(_objRect.bottom);

	isActive = _active;
	objSize = _objSize;
	objType = _objType;
	collisionDirection = _collisionDirection;
	objRect = _objRect;
	mPos = _position;
	mScale = _scale;

	collisionBounds.left = mPos.x;
	collisionBounds.top = mPos.y;
	collisionBounds.right = mPos.x + objSize.x * mScale.x;
	collisionBounds.bottom = mPos.y + objSize.y * mScale.y;

	dManager->GetResourceUpload()->Begin();

	DX::ThrowIfFailed(
		DirectX::CreateDDSTextureFromFile(dManager->GetDevice(), *dManager->GetResourceUpload(), texPath.c_str(),
			objTex.ReleaseAndGetAddressOf()));

	DirectX::CreateShaderResourceView(dManager->GetDevice(), objTex.Get(),
		dManager->GetResourceDescriptors()->GetCpuHandle(1));

	auto uploadResourcesFinished = dManager->GetResourceUpload()->End(
		dManager->GetDeviceResources()->GetCommandQueue());
	uploadResourcesFinished.wait();
}

void Tile::Render(DeviceManager* dManager)
{
	RECT* sourceRect = &objRect;

	dManager->GetSpriteBatch()->Draw(dManager->GetResourceDescriptors()->GetGpuHandle(1),
		DirectX::GetTextureSize(objTex.Get()),
		mPos, sourceRect, { 1.f, 1.f, 1.f, 1.f }, 0.f, mOrigin, mScale);
}

void Tile::SetActive(bool _isActive)
{
	isActive = _isActive;
}

void Tile::SetObjectSize(DirectX::SimpleMath::Vector2 _objSize)
{
	objSize = _objSize;
}

void Tile::SetCollisionDirection(int _direction)
{
	collisionDirection = _direction;
}

void Tile::SetRect(RECT _objRect)
{
	objRect = _objRect;
}

void Tile::SetPosition(DirectX::SimpleMath::Vector2 _position)
{
	mPos = _position;
}

void Tile::SetScale(DirectX::SimpleMath::Vector2 _scale)
{
	mScale = _scale;
}