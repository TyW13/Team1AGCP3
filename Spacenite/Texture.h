#pragma once
#include "WindowUtils.h"
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
		// path is data/
		path.substr(0, 1);
		auto itr = path.find_last_of(".");
		std::string noSuff = path.substr(0, itr);
		auto lastSlash = noSuff.find_last_of("/");
		std::string noPath = noSuff.substr(lastSlash+1, noSuff.length() - lastSlash);
		tName = noPath;
	}

	std::string tName;
	std::string tPath;
	Vector2 tDimensions;
};