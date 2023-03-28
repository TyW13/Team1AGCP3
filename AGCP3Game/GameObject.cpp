#include "GameObject.h"
#include "ResourceManager.h"

void GameObject::Init(DeviceManager* dManager, std::wstring texPath, DirectX::SimpleMath::Vector2 _position, DirectX::SimpleMath::Vector2 _scale, bool _active, DirectX::SimpleMath::Vector2 _objSize, std::string _objType, bool _isCollidable, RECT _objRect)
{
	std::string tileRectsString = std::to_string(_objRect.left) + std::to_string(_objRect.top) + std::to_string(_objRect.right) + std::to_string(_objRect.bottom);

	isActive = _active;
	objSize = _objSize;
	objType = _objType;
	isCollidable = _isCollidable;
	objRect = _objRect;
	mPos = _position;
	mScale = _scale;

	collisionBounds.left = mPos.x;
	collisionBounds.top = mPos.y;
	collisionBounds.right = mPos.x + objSize.x * mScale.x;
	collisionBounds.bottom = mPos.y + objSize.y * mScale.y;

	dManager->GetResourceUpload()->Begin();																				// Creating texture

	DX::ThrowIfFailed(
		DirectX::CreateDDSTextureFromFile(dManager->GetDevice(), *dManager->GetResourceUpload(), texPath.c_str(),
			objTex.ReleaseAndGetAddressOf()));

	DirectX::CreateShaderResourceView(dManager->GetDevice(), objTex.Get(),
		dManager->GetResourceDescriptors()->GetCpuHandle(1));

	auto uploadResourcesFinished = dManager->GetResourceUpload()->End(
		dManager->GetDeviceResources()->GetCommandQueue());

	uploadResourcesFinished.wait();
}

void GameObject::Update(DeviceManager* dManager, ResourceManager* rManager, float dTime)
{

}

void GameObject::Render(DeviceManager* dManager)
{
	RECT* sourceRect = &objRect;

	dManager->GetSpriteBatch()->Draw(dManager->GetResourceDescriptors()->GetGpuHandle(1),
		DirectX::GetTextureSize(objTex.Get()),
		mPos, sourceRect, { 1.f, 1.f, 1.f, 1.f }, 0.f, dManager->GetOrigin(), mScale);
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


//// BULLET / AI
//
//DirectX::SimpleMath::Vector2 GameObject::GetVelocity() const
//{
//	return DirectX::SimpleMath::Vector2();
//}
//
//DirectX::SimpleMath::Vector2 GameObject::SetVelocity(float x, float y)
//{
//	m_bulletVelocity.y = y;
//	m_bulletVelocity.y = x;
//}

//void GameObject::SetPosition(DirectX::XMFLOAT2 position)
//{
//	m_position = position;
//}
//
//DirectX::XMFLOAT2 GameObject::GetPosition(float x, float y)
//{
//	return m_position;
//}

//DirectX::XMFLOAT2 GameObject::GetPosition() const;
//{
//	return m_position;
//}


