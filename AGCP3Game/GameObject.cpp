#include "GameObject.h"

void GameObject::Init(DeviceManager* d3d, std::wstring texPath, DirectX::SimpleMath::Vector2 _position, DirectX::SimpleMath::Vector2 _scale, bool _active, DirectX::SimpleMath::Vector2 _objSize, std::string _objType, bool _isCollidable, RECT _objRect)
{
	std::string tileRectsString = std::to_string(_objRect.left) + std::to_string(_objRect.top) + std::to_string(_objRect.right) + std::to_string(_objRect.bottom);

	std::vector<RECT>* tempTileRect = new std::vector<RECT>;
	tempTileRect->push_back(_objRect);

	objRect = _objRect;
	mPos = _position;
	mScale = _scale;

	collisionBounds.left = mPos.x;
	collisionBounds.top = mPos.y;
	collisionBounds.right = mPos.x + objSize.x * mScale.x;
	collisionBounds.bottom = mPos.y + objSize.y * mScale.y;

	d3d->GetResourceUpload()->Begin();																				// Creating texture

	DX::ThrowIfFailed(
		DirectX::CreateDDSTextureFromFile(d3d->GetDevice(), *d3d->GetResourceUpload(), texPath.c_str(),
			objTex.ReleaseAndGetAddressOf()));

	DirectX::CreateShaderResourceView(d3d->GetDevice(), objTex.Get(),
		d3d->GetResourceDescriptors()->GetCpuHandle(1));

	auto uploadResourcesFinished = d3d->GetResourceUpload()->End(
		d3d->GetDeviceResources()->GetCommandQueue());

	uploadResourcesFinished.wait();
}

void GameObject::Render(DeviceManager* mD3D)
{
	RECT* sourceRect = &objRect;

	mD3D->GetSpriteBatch()->Draw(mD3D->GetResourceDescriptors()->GetGpuHandle(1),
		DirectX::GetTextureSize(objTex.Get()),
		mPos, sourceRect, { 1.f, 1.f, 1.f, 1.f }, 0.f, mD3D->GetOrigin(), mScale);
}

void GameObject::SetActive(bool _isActive)
{
	isActive = _isActive;
}

void GameObject::SetObjectSize(DirectX::SimpleMath::Vector2 _objSize)
{
	objSize = _objSize;
}

void GameObject::SetIsCollidable(bool _isCollidable)
{
	isCollidable = _isCollidable;
}

void GameObject::SetRect(RECT _objRect)
{
	objRect = _objRect;
}

void GameObject::SetPosition(DirectX::SimpleMath::Vector2 _position)
{
	mPos = _position;
}

void GameObject::SetScale(DirectX::SimpleMath::Vector2 _scale)
{
	mScale = _scale;
}