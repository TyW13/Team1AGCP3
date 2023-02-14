#pragma once
#include "newGameObject.h"

class Tile : public GameObject
{
public:
	Tile(MyD3D& d3d, Texture* objTex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, RECTF tileRect) : GameObject(d3d), objSprite(d3d)
	{
		Init(objTex, scale, position, active, tileRect);
	}

	void Init(Texture* tex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, RECTF tileRect);

	Sprite GetSprite() override;
	bool GetActive() override;
	void SetSprite(Sprite _sprite) override;
	void SetActive(bool _isActive) override;
private:
	Sprite objSprite;
	bool isActive;

	std::vector<RECTF> ownTileRect;
};

