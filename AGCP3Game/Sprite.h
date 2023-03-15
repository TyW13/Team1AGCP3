#pragma once
#include "pch.h"


class Sprite
{
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> mpTex;
	RECT mTexRect;
	DirectX::SimpleMath::Vector2 scale;
	
public:
	DirectX::SimpleMath::Vector2 mPos;
	DirectX::SimpleMath::Vector2 mVel;
	float depth;
	DirectX::SimpleMath::Vector4 colour;
	float rotation;
	DirectX::SimpleMath::Vector2 origin;

	Sprite()
		: mPos(0,0), mVel(0,0), depth(0), mTexRect{0,0,0,0},
		colour(1,1,1,1), rotation(0), scale(1,1), origin(0,0)
	{}
	Sprite(const Sprite& rhs)
	{
		*(this) = rhs;
	}
	Sprite& operator=(const Sprite& rhs)
	{
		mpTex = rhs.mpTex;
		mTexRect = rhs.mTexRect;
		scale = rhs.scale;
		mPos = rhs.mPos;
		mVel = rhs.mVel;
		depth = rhs.depth;
		colour = rhs.colour;
		rotation = rhs.rotation;
		origin = rhs.origin;
		return *this;
	}

	void Draw(DirectX::SpriteBatch& batch);
	void SetTex(ID3D12Resource& tex, const RECT& texRect = RECT{ 0,0,0,0 });
	void SetTexRect(const RECT& texRect);


	// Getters
	RECT GetTexRect() { return mTexRect; }
	ID3D12Resource& GetTex()
	{
		assert(mpTex);
		return *mpTex.Get();
	}
	const DirectX::SimpleMath::Vector2& GetScale() const
	{
		return scale;
	}

	// Setters
	void SetScale(const DirectX::SimpleMath::Vector2& s)
	{
		scale = s;
	}
};