#include "GameObject.h"
#include "ResourceManager.h"

void GameObject::Init(DeviceManager* dManager, int _texId, DirectX::SimpleMath::Vector2 _position, DirectX::SimpleMath::Vector2 _scale, float _rotation, bool _active, DirectX::SimpleMath::Vector2 _objSize, std::string _objType, bool _isCollidable, RECT _objRect)
{
	std::string tileRectsString = std::to_string(_objRect.left) + std::to_string(_objRect.top) + std::to_string(_objRect.right) + std::to_string(_objRect.bottom);

	isActive = _active;
	objSize = _objSize;
	objType = _objType;
	isCollidable = _isCollidable;
	objRect = _objRect;
	mPos = _position;
	mScale = _scale;
	mRotation = _rotation;
	texId = _texId;

	collisionBounds.left = mPos.x;
	collisionBounds.top = mPos.y;
	collisionBounds.right = mPos.x + objSize.x * mScale.x;
	collisionBounds.bottom = mPos.y + objSize.y * mScale.y;

	//dManager->GetResourceUpload()->Begin();																				// Creating texture

	//DX::ThrowIfFailed(
	//	DirectX::CreateDDSTextureFromFile(dManager->GetDevice(), *dManager->GetResourceUpload(), texPath.c_str(),
	//		objTex.ReleaseAndGetAddressOf()));

	//DirectX::CreateShaderResourceView(dManager->GetDevice(), objTex.Get(),
	//	dManager->GetResourceDescriptors()->GetCpuHandle(1));

	//auto uploadResourcesFinished = dManager->GetResourceUpload()->End(
	//	dManager->GetDeviceResources()->GetCommandQueue());

	//uploadResourcesFinished.wait();
}

void GameObject::Update(DeviceManager* dManager, ResourceManager* rManager, float dTime)
{

}

void GameObject::Render(DeviceManager* dManager, ResourceManager& resourceManager)
{
	RECT* sourceRect = &objRect;

	dManager->GetSpriteBatch()->Draw(dManager->GetResourceDescriptors()->GetGpuHandle(texId),
		DirectX::GetTextureSize(resourceManager.GetTextures()[texId].resource),
		mPos, sourceRect, { 1.f, 1.f, 1.f, 1.f }, mRotation, dManager->GetOrigin(), mScale);
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