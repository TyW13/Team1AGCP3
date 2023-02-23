#pragma once
#include "newGameObject.h"

class Tile : public GameObject
{
public:
	Tile(MyD3D& d3d, Texture* objTex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, RECTF tileRect, Vector2 collisionBounds, int objnum)
		: GameObject(d3d, objTex, position, scale, active, tileRect, collisionBounds, objnum), objSprite(d3d)
	{
		Init(objTex, position, scale, active, tileRect, collisionBounds, objnum);
	}

	void Init(Texture* tex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, RECTF tileRect, Vector2 collisionBounds, int objnum) override;

	Sprite GetSprite() override;
	bool GetActive() override;
	void SetSprite(Sprite _sprite) override;
	void SetActive(bool _isActive) override;
private:
	Sprite objSprite;
	bool isActive;

	std::vector<RECTF> ownTileRect;
};

