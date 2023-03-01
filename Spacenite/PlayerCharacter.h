#pragma once

#include "newGameObject.h"

class Sprite;
class ResourceManager;

class PlayerCharacter : public GameObject
{
public:
	PlayerCharacter(MyD3D& d3d, Texture* objTex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, RECTF tileRect, Vector2 collisionBounds, int objnum)	// Default constructor
		: GameObject(d3d, objTex, position, scale, active, tileRect, collisionBounds, objnum), objSprite(d3d)
	{
		Init(objTex, position, scale, active, tileRect, collisionBounds, objnum);
	}

	void Init(Texture* tex, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, RECTF tileRect, Vector2 collisionBounds, int objnum) override;
	void Update(float dTime, ResourceManager& rManager) override;
	void Terminate();

	Sprite GetSprite() override;
	bool GetActive() override;
	void SetSprite(Sprite _sprite) override;
	void SetActive(bool _isActive) override;

	RECT GetPlayerCollisionBounds() { return collisionPlayerRect; }

private:
	bool isActive;
	Sprite objSprite;

	void UpdateInput(float dTime);									//PLAYER EXCLUSIVE
	void CheckCollision(ResourceManager& rManager);											// temporary collision function, Kieron can swap it for his collision once merged

	const float JUMP_SPEED = 1200;
	const float GRAVITY = 400;
	const float PLAYER_SPEED = 350;									//PLAYER EXCLUSIVE
	const float JUMP_HEIGHT = 100;
	const float PAD_SPEED = 500;									//PLAYER EXCLUSIVE
	const float DRAG = 0.985;										//for deceleration

	bool isGrounded = false;													//Player
	bool isTop = false;			// For kacper temp collision
	bool isBottom = false;		// For kacper temp collision

	RECT collisionPlayerRect;

	Vector2 collisionDimensions;
};