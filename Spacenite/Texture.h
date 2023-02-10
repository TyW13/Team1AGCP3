#pragma once
#include "WindowUtils.h"
#include "Input.h"
#include "D3DUtil.h"
#include <assert.h>
#include <DDSTextureLoader.h>

using namespace DirectX::SimpleMath;

class Texture
{
public:
	Texture(){}
	Texture(const std::string& path)
	{
		Init(path);
	}

	// Commented out unneeded copy and move constructors and asssignment operators

	//Texture(const Texture& other)																								// Copy constructor
	//{
	//	this->tName = other.tName;
	//	this->tPath = other.tPath;
	//}
	//
	//Texture& operator=(Texture& other)
	//{
	//	if (this != &other)
	//	{
	//		this->tName = other.tName;
	//		this->tPath = other.tPath;
	//	}
	//
	//	return *this;
	//}
	//
	//Texture(Texture&& other) noexcept																		// Move constructor
	//{
	//	this->tName = other.tName;
	//	this->tPath = other.tPath;
	//}
	//
	//Texture& operator=(Texture&& other)	noexcept																// Move assignment operator
	//{
	//	if (this != &other)
	//	{
	//		this->tName = other.tName;
	//		this->tPath = other.tPath;
	//	}
	//
	//	return *this;
	//}
	//
	//~Texture()																											// Destructor
	//{
	//	printf("DELETING TEXTURE OBJECT\n");
	//}

	void Init(const std::string& path);

	std::string GetName()
	{
		return tName;
	}

	std::string GetPath() 
	{
		return tPath;
	}

	Vector2 GetDimensions() 
	{
		return tDimensions;
	}

private:
	void SetName(std::string path)
	{
		// path is data/textures/
		path.substr(0, 1);
		auto itr = path.find_last_of(".");
		std::string noSuff = path.substr(0, itr);
		auto lastSlash = noSuff.find_last_of("/");
		std::string noPath = noSuff.substr(lastSlash+1, noSuff.length() - lastSlash);
		tName = noPath;
	}

	//void SetPath(std::string path)
	//{
	//	tPath = "data/textures/" + path;
	//}

	//void SetDimensions()
	//{
	//	assert(m_pTexture);
	//	ID3D11Resource* res = nullptr;
	//	m_pTexture->GetResource(&res);
	//	assert(res);
	//	ID3D11Texture2D* texture2d = nullptr;
	//	HRESULT hr = res->QueryInterface(&texture2d);
	//	if (SUCCEEDED(hr))
	//	{
	//		D3D11_TEXTURE2D_DESC desc;
	//		texture2d->GetDesc(&desc);
	//		tDimensions.x = static_cast<float>(desc.Width);
	//		tDimensions.y = static_cast<float>(desc.Height);
	//	}
	//	ReleaseCOM(texture2d);
	//	ReleaseCOM(res);
	//}

	//ID3D11ShaderResourceView* m_pTexture;
	std::string tName;
	std::string tPath;
	Vector2 tDimensions;
};