#pragma once
#include "newGameObject.h"

class TileMap : public GameObject
{
public:
	TileMap(MyD3D& d3d, Texture* objTex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active) : GameObject(d3d), objSprite(d3d)
	{
		Init(objTex, scale, position, active);
	}

	void Init(Texture* tex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active) override;

	Sprite GetSprite() override;
	bool GetActive() override;
	void SetSprite(Sprite _sprite) override;
	void SetActive(bool _isActive) override;
private:
	Sprite objSprite;
	bool isActive;
};

