#include "Player.h"

void Player::Init(DeviceManager* dManager, std::wstring texPath, DirectX::SimpleMath::Vector2 _position, DirectX::SimpleMath::Vector2 _scale, bool _active, DirectX::SimpleMath::Vector2 _objSize, std::string _objType, bool _isCollidable, RECT _objRect)
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

	dManager->GetResourceUpload()->Begin();																					// Start of texture loading

	DX::ThrowIfFailed(																										// Error check for creation of dds texture
		DirectX::CreateDDSTextureFromFile(dManager->GetDevice(), *dManager->GetResourceUpload(), texPath.c_str(),
			objTex.ReleaseAndGetAddressOf()));

	DirectX::CreateShaderResourceView(dManager->GetDevice(), objTex.Get(),													// Creation of shader resource view?
		dManager->GetResourceDescriptors()->GetCpuHandle(0));

	auto uploadResourcesFinished = dManager->GetResourceUpload()->End(
		dManager->GetDeviceResources()->GetCommandQueue());
	uploadResourcesFinished.wait();
}

void Player::Update(float dTime)
{
	PlayerAnim.Update(dTime, this.)
	// PUT PLAYER UPDATE STUFF IN HERE!
}

void Player::Render(DeviceManager* dManager)
{
	RECT* sourceRect = &objRect;

	dManager->GetSpriteBatch()->Draw(dManager->GetResourceDescriptors()->GetGpuHandle(0),
		DirectX::GetTextureSize(objTex.Get()),
		mPos, sourceRect, { 1.f, 1.f, 1.f, 1.f }, 0.f, mOrigin, mScale);
}

void Player::SetActive(bool _isActive)
{
	isActive = _isActive;
}

void Player::SetObjectSize(DirectX::SimpleMath::Vector2 _objSize)
{
	objSize = _objSize;
}

void Player::SetIsCollidable(bool _isCollidable)
{
	isCollidable = _isCollidable;
}

void Player::SetRect(RECT _objRect)
{
	objRect = _objRect;
}

void Player::SetPosition(DirectX::SimpleMath::Vector2 _position)
{
	mPos = _position;
}

void Player::SetScale(DirectX::SimpleMath::Vector2 _scale)
{
	mScale = _scale;
}