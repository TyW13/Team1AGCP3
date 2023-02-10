#pragma once

#include "D3D.h"
#include <SpriteFont.h>
#include "Sprite.h"
#include "Texture.h"

class GameObject
{
public:
	GameObject(MyD3D& d3d)
		: objSprite(d3d)
	{
		//Init();
	}

	// Commented out Copy and move constructors and assignment operators

	//GameObject(const GameObject& other)	: objSprite(other.objSprite)															// Copy constructor
	//{
	//	this->isActive = other.isActive;
	//	this->objSprite = other.objSprite;
	//}
	//
	//GameObject& operator=(const GameObject& other)																		// Copy assignment operator
	//{
	//	if (this != &other)
	//	{
	//		this->isActive = other.isActive;
	//		this->objSprite = other.objSprite;
	//	}
	//
	//	return *this;
	//}
	//
	//GameObject(GameObject&& other) noexcept : objSprite(other.GetSprite())										// Move constructor
	//{
	//	this->isActive = other.isActive;
	//	this->objSprite = other.objSprite;
	//}
	//
	//GameObject& operator=(GameObject&& other)	noexcept																// Move assignment operator
	//{
	//	if (this != &other)
	//	{
	//		this->isActive = other.isActive;
	//		this->objSprite = other.objSprite;
	//	}
	//
	//	return *this;
	//}
	//
	//~GameObject()																											// Destructor
	//{
	//	printf("DELETING GAME OBJECT\n");
	//}	

	virtual void Init();
	void Update();
	void Terminate();

	virtual Sprite GetSprite();
	virtual bool GetActive();
	virtual void SetSprite(Sprite _sprite);
	virtual void SetActive(bool _isActive);
private:
	Sprite objSprite;
	bool isActive;
};

