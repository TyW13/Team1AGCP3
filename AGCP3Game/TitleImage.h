#pragma once

#include "DeviceManager.h"

class TitleImage
{
public:

	//TitleImage(){}
	TitleImage(DeviceManager* dManager, std::string texPath, DirectX::SimpleMath::Vector2 _position, DirectX::SimpleMath::Vector2 _scale) 
	{
		Init(dManager, texPath, _position, _scale);
	}
	~TitleImage(){}

	void Init(DeviceManager* dManager, std::string texPath, DirectX::SimpleMath::Vector2 _position, DirectX::SimpleMath::Vector2 _scale);
	void Render(DeviceManager* dManager);

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> objTex;
	DirectX::SimpleMath::Vector2 mPos = {};
	DirectX::SimpleMath::Vector2 mScale = {};
	DirectX::SimpleMath::Vector2 mOrigin = { 0,0 };
};

