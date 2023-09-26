//#include "Player.h"
#include "ResourceManager.h"
#include "Shotgun.h"
#include <math.h> 

void Shotgun::Init(DeviceManager* dManager, std::wstring texPath, DirectX::SimpleMath::Vector2 _position, DirectX::SimpleMath::Vector2 _scale, bool _active, DirectX::SimpleMath::Vector2 _objSize, std::string _objType, int _collisionDirection, RECT _objRect)
{
	std::string tileRectsString = std::to_string(_objRect.left) + std::to_string(_objRect.top) + std::to_string(_objRect.right) + std::to_string(_objRect.bottom);

	isActive = _active;
	objSize = _objSize;
	objType = _objType;
	collisionDirection = _collisionDirection;
	objRect = _objRect;
	mPos = _position;
	mScale = _scale;

	dManager->GetResourceUpload()->Begin();

	DX::ThrowIfFailed(
		DirectX::CreateDDSTextureFromFile(dManager->GetDevice(), *dManager->GetResourceUpload(), texPath.c_str(),
			objTex.ReleaseAndGetAddressOf()));

	DirectX::CreateShaderResourceView(dManager->GetDevice(), objTex.Get(),
		dManager->GetResourceDescriptors()->GetCpuHandle(2));

	auto uploadResourcesFinished = dManager->GetResourceUpload()->End(
		dManager->GetDeviceResources()->GetCommandQueue());
	uploadResourcesFinished.wait();

	Anim.Init("Shotgun.json", *this);
}

void Shotgun::Update(DeviceManager* dManager, ResourceManager* rManager, float dTime)
{
	//Changes the origin to the center of the shotgun
	mOrigin = DirectX::SimpleMath::Vector2(this->GetObjectSize().x / 2, this->GetObjectSize().y / 2);

	//Sets Shotgun position to be attached to the player but at an offset away from them
	DirectX::SimpleMath::Vector2 playerPos = rManager->GetPlayer()->GetPosition();
	mPos = playerPos + PosOffset;

	//Gets the cursors position and uses it and the shotguns position to find the angle the shotgun needs to point at using trigonometry
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	float DistX = cursorPos.x - mPos.x;
	float DistY = cursorPos.y - mPos.y;
	Rotation = atan(DistY / DistX);
	//If the x is negative it rotates the gun 180 degrees
	if (DistX < 0)
	{
		Rotation += Rads180;
	}

	//Checks to make sure the gun should appear and animate
	if (rManager->GetPlayer()->GetGrounded() == true && AnimState == 1)
	{
		AnimState = 0;
	}
	if (rManager->GetPlayer()->GetFired() == true)
	{
		AnimState = 1;
	}
	Anim.Update(dTime, *this, AnimState);
}

void Shotgun::Render(DeviceManager* dManager)
{
	RECT* sourceRect = &objRect;

	dManager->GetSpriteBatch()->Draw(dManager->GetResourceDescriptors()->GetGpuHandle(2),
		DirectX::GetTextureSize(objTex.Get()),	
		mPos, sourceRect, { 1.f, 1.f, 1.f, 1.f }, Rotation, mOrigin, mScale);
}

void Shotgun::SetActive(bool _isActive)
{
	isActive = _isActive;
}

void Shotgun::SetObjectSize(DirectX::SimpleMath::Vector2 _objSize)
{
	objSize = _objSize;
}

void Shotgun::SetCollisionDirection(int _direction)
{
	collisionDirection = _direction;
}

void Shotgun::SetRect(RECT _objRect)
{
	objRect = _objRect;
}

void Shotgun::SetPosition(DirectX::SimpleMath::Vector2 _position)
{
	mPos = _position;
}

void Shotgun::SetScale(DirectX::SimpleMath::Vector2 _scale)
{
	mScale = _scale;
}