#pragma once
#include "D3DUtil.h"
#include "D3D.h"
#include <SpriteFont.h>
#include "Sprite.h"
#include "newGameObject.h"

class Sprite;

class PlayerCharacter : public GameObject
{
public:
	PlayerCharacter(MyD3D& d3d, Texture objTex, DirectX::SimpleMath::Vector2 scale, bool active)	// Default constructor
		: GameObject(d3d), objSprite(d3d)
	{
		Init(objTex, scale, active);
	}

	PlayerCharacter(const PlayerCharacter& other) : GameObject(other), objSprite(other.objSprite)									// Copy constructor
	{
		this->isActive = other.isActive;
		this->objSprite = other.objSprite;
	}

	PlayerCharacter& operator=(const PlayerCharacter& other)																		// Copy assignment operator
	{
		if (this != &other)
		{
			this->isActive = other.isActive;
			this->objSprite = other.objSprite;
		}

		return *this;
	}

	PlayerCharacter(PlayerCharacter&& other) noexcept : GameObject(other), objSprite(other.objSprite)									// Move constructor
	{
		this->isActive = other.isActive;
		this->objSprite = other.objSprite;
	}

	PlayerCharacter& operator=(PlayerCharacter&& other)	noexcept																// Move assignment operator
	{
		if (this != &other)
		{
			this->isActive = other.isActive;
			this->objSprite = other.objSprite;
		}

		return *this;
	}

	~PlayerCharacter(){}																											// Destructor

	void Init(Texture tex, DirectX::SimpleMath::Vector2 scale, bool active);

	Sprite GetSprite() override;
	bool GetActive() override;
	void SetSprite(Sprite _sprite) override;
	void SetActive(bool _isActive) override;
private:
	bool isActive;
	Sprite objSprite;
};