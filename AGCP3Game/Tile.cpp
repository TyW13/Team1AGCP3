#include "Tile.h"

// Device, resourceUplaod, resourceDecriptor, decriptors can go in "d3d"

void Tile::Init(DeviceManager* d3d, DirectX::SimpleMath::Vector2 _position, DirectX::SimpleMath::Vector2 _scale, bool _active, DirectX::SimpleMath::Vector2 _objSize, std::string _objType, bool _isCollidable, RECT _objRect)
{
	std::string tileRectsString = std::to_string(_objRect.left) + std::to_string(_objRect.top) + std::to_string(_objRect.right) + std::to_string(_objRect.bottom);

	std::vector<RECT>* tempTileRect = new std::vector<RECT>;
	tempTileRect->push_back(_objRect);

	objRect = _objRect;
	mPos = _position;
	mScale = _scale;

	d3d->GetResourceUpload()->Begin();

	DX::ThrowIfFailed(
		DirectX::CreateDDSTextureFromFile(d3d->GetDevice(), *d3d->GetResourceUpload(), L"Data/test_sheet2.dds",
			objTex.ReleaseAndGetAddressOf()));

	DirectX::CreateShaderResourceView(d3d->GetDevice(), objTex.Get(),
		d3d->GetResourceDescriptors()->GetCpuHandle(1));

	auto uploadResourcesFinished = d3d->GetResourceUpload()->End(
		d3d->GetDeviceResources()->GetCommandQueue());

	uploadResourcesFinished.wait();

	//ID3D11ShaderResourceView* objTex = objSprite.GetD3D().GetCache().LoadTexture(&objSprite.GetD3D().GetDevice(), _tex->GetPath(), _tex->GetName() + tileRectsString, true, tempTileRect);
	//objSprite.SetTex(*objTex);

	//objSprite.SetScale(_scale);
	//objSprite.origin = DirectX::SimpleMath::Vector2(0, 0);					// Swap between (0 ,0) or (0, 8) to test which looks better (fits all the tiles correctly on y axis)
	//isActive = _active;

	//objSprite.GetAnim().Init(0, 0, 0, true);

	//objSprite.mPos = _position;
	//objSize = _objSize;
	//objType = _objType;
	//isCollidable = _isCollidable;

	//collisionBounds.left = objSprite.mPos.x;
	//collisionBounds.top = objSprite.mPos.y;
	//collisionBounds.right = objSprite.mPos.x + objSize.x * _scale.x;
	//collisionBounds.bottom = objSprite.mPos.y + objSize.y * _scale.y;
}

void Tile::Render(DeviceManager* mD3D)
{
	RECT* sourceRect = &objRect;

	mD3D->GetSpriteBatch()->Draw(mD3D->GetResourceDescriptors()->GetGpuHandle(1),
		DirectX::GetTextureSize(objTex.Get()),
		mPos, sourceRect, { 1.f, 1.f, 1.f, 1.f }, 0.f, mD3D->GetOrigin(), mScale);
}

bool Tile::GetActive()
{
	return isActive;
}

void Tile::SetActive(bool _isActive)
{
	isActive = _isActive;
}

void Tile::SetIsCollidable(bool _isCollidable)
{
	isCollidable = _isCollidable;
}