//#pragma once
//#include "pch.h"
//
//class Tile
//{
//public:
//	Tile(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, DirectX::SimpleMath::Vector2 objSize, std::string objType, bool isCollidable, RECT objRect = { 0,0 ,0 ,0 })
//	{
//		Init(position, scale, active, objSize, objType, isCollidable, objRect);
//	}
//
//	void Init(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, DirectX::SimpleMath::Vector2 objSize, std::string _objType, bool isCollidable, RECT objRect = { 0,0 ,0 ,0 });
//
//	bool GetActive();
//	DirectX::SimpleMath::Vector2 GetObjectSize() { return objSize; }
//	std::string GetObjectType() { return objType; }
//	bool GetIsCollidable() { return isCollidable; }
//	RECT GetCollisionBounds() { return collisionBounds; }
//
//	void SetActive(bool _isActive);
//	void SetObjectSize(DirectX::SimpleMath::Vector2 _objSize);
//	void SetIsCollidable(bool _isCollidable);
//private:
//	Microsoft::WRL::ComPtr<ID3D12Resource> objTex;
//	bool isActive;
//	DirectX::SimpleMath::Vector2 objSize;
//	std::string objType;
//	bool isCollidable;
//
//	RECT collisionBounds;
//};
//
