//#include "Tile.h"
//#include "DDSTextureLoader.h"
//#include "GameRenderer.h"
//
//// Device, resourceUplaod, resourceDecriptor, decriptors can go in "d3d"
//
//void Tile::Init(DirectX::SimpleMath::Vector2 _position, DirectX::SimpleMath::Vector2 _scale, bool _active, DirectX::SimpleMath::Vector2 _objSize, std::string _objType, bool _isCollidable, RECT _objRect)
//{
//	std::string tileRectsString = std::to_string(_objRect.left) + std::to_string(_objRect.top) + std::to_string(_objRect.right) + std::to_string(_objRect.bottom);
//
//	std::vector<RECT>* tempTileRect = new std::vector<RECT>;
//	tempTileRect->push_back(_objRect);
//
//	DX::ThrowIfFailed(
//		DirectX::CreateDDSTextureFromFile(device, resourceUpload, L"Data/test_sheet2.dds",
//			objTex.ReleaseAndGetAddressOf()));
//
//	DirectX::CreateShaderResourceView(device, objTex.Get(),
//		m_resourceDescriptors->GetCpuHandle(Descriptors::Tile));
//
//	//ID3D11ShaderResourceView* objTex = objSprite.GetD3D().GetCache().LoadTexture(&objSprite.GetD3D().GetDevice(), _tex->GetPath(), _tex->GetName() + tileRectsString, true, tempTileRect);
//	//objSprite.SetTex(*objTex);
//
//	//objSprite.SetScale(_scale);
//	//objSprite.origin = DirectX::SimpleMath::Vector2(0, 0);					// Swap between (0 ,0) or (0, 8) to test which looks better (fits all the tiles correctly on y axis)
//	//isActive = _active;
//
//	//objSprite.GetAnim().Init(0, 0, 0, true);
//
//	//objSprite.mPos = _position;
//	//objSize = _objSize;
//	//objType = _objType;
//	//isCollidable = _isCollidable;
//
//	//collisionBounds.left = objSprite.mPos.x;
//	//collisionBounds.top = objSprite.mPos.y;
//	//collisionBounds.right = objSprite.mPos.x + objSize.x * _scale.x;
//	//collisionBounds.bottom = objSprite.mPos.y + objSize.y * _scale.y;
//}
//
//bool Tile::GetActive()
//{
//	return isActive;
//}
//
//void Tile::SetActive(bool _isActive)
//{
//	isActive = _isActive;
//}
//
//void Tile::SetIsCollidable(bool _isCollidable)
//{
//	isCollidable = _isCollidable;
//}