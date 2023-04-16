//#include "Gem.h"
//#include "ResourceManager.h"
//
//
//void Gem::Init(DeviceManager* dManager, std::wstring texPath, DirectX::SimpleMath::Vector2 _position, DirectX::SimpleMath::Vector2 _scale, bool _active, DirectX::SimpleMath::Vector2 _objSize, std::string _objType, bool _isCollidable, RECT _objRect)
//{
//	std::string tileRectsString = std::to_string(_objRect.left) + std::to_string(_objRect.top) + std::to_string(_objRect.right) + std::to_string(_objRect.bottom);
//
//	isActive = _active;
//	objSize = _objSize;
//	objType = _objType;
//	isCollidable = _isCollidable;
//	objRect = _objRect;
//	mPos = _position;
//	mScale = _scale;
//
//	dManager->GetResourceUpload()->Begin();																					// Start of texture loading
//
//	DX::ThrowIfFailed(																										// Error check for creation of dds texture
//		DirectX::CreateDDSTextureFromFile(dManager->GetDevice(), *dManager->GetResourceUpload(), texPath.c_str(),
//			objTex.ReleaseAndGetAddressOf()));
//
//	DirectX::CreateShaderResourceView(dManager->GetDevice(), objTex.Get(),													// Creation of shader resource view?
//		dManager->GetResourceDescriptors()->GetCpuHandle(0));
//
//	auto uploadResourcesFinished = dManager->GetResourceUpload()->End(
//		dManager->GetDeviceResources()->GetCommandQueue());
//	uploadResourcesFinished.wait();
//}
//
//
//void Gem::Update(DeviceManager* dManager, ResourceManager* rManager, float dTime)
//{
//	collisionBounds.left = mPos.x;
//	collisionBounds.top = mPos.y;
//	collisionBounds.right = mPos.x + objSize.x * mScale.x;
//	collisionBounds.bottom = mPos.y + objSize.y * mScale.y;
//
//	CheckCollision(dManager, rManager, dTime);
//}
//
//void Gem::Render(DeviceManager* dManager)
//{
//	RECT* sourceRect = &objRect;
//
//	dManager->GetSpriteBatch()->Draw(dManager->GetResourceDescriptors()->GetGpuHandle(0),
//		DirectX::GetTextureSize(objTex.Get()),
//		mPos, sourceRect, { 1.f, 1.f, 1.f, 1.f }, 0.f, mOrigin, mScale);
//}
//
//void Gem::CheckCollision(DeviceManager* dManager, ResourceManager* rManager, float dTime)
//{
//	collidedTop = false;
//	collidedBottom = false;
//	collidedLeft = false;
//	collidedRight = false;
//
//}
//
//void Gem::SetActive(bool _isActive)
//{
//	isActive = _isActive;
//}
//
//void Gem::SetObjectSize(DirectX::SimpleMath::Vector2 _objSize)
//{
//	objSize = objSize;
//}
//
//void Gem::SetIsCollidable(bool _isCollidable)
//{
//	isCollidable = _isCollidable;
//}
//
//void Gem::SetRect(RECT _objRect)
//{
//	objRect = _objRect;
//}
//
//void Gem::SetPosition(DirectX::SimpleMath::Vector2 _position)
//{
//	mPos = _position;
//}
//
//void Gem::SetScale(DirectX::SimpleMath::Vector2 _scale)
//{
//	mScale = _scale;
//}
