#pragma once
#include "GameObject.h"
#include "Game.h"
#include "WindowUtils.h"
#include "Input.h"
#include "D3DUtil.h"
#include <assert.h>
#include <DDSTextureLoader.h>

using namespace DirectX::SimpleMath;

class Texture
{
public:
	Texture(ID3D11Device* pDevice, const std::string& path) 
	{
		//DirectX::DDS_ALPHA_MODE alpha;
		//if(DirectX::CreateDDSTextureFromFile(pDevice, path.c_str(), nullptr, &m_pTexture, 0, &alpha) != S_OK)
		//{
		//	WDBOUT("CANNOT LOAD " << path.c_str() << "\n");
		//	assert(false);
		//}
		SetName(path);
		SetPath(path);
		SetDimensions();
	}

	void Init();
	void Update();
	void Terminate();

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
		std::string noPath = noSuff.substr(lastSlash, noSuff.length() - lastSlash);
		tName = noPath;
	}

	void SetPath(std::string path)
	{
		tPath = "data/textures/" + path;
	}

	void SetDimensions()
	{
		assert(m_pTexture);
		ID3D11Resource* res = nullptr;
		m_pTexture->GetResource(&res);
		assert(res);
		ID3D11Texture2D* texture2d = nullptr;
		HRESULT hr = res->QueryInterface(&texture2d);
		if (SUCCEEDED(hr))
		{
			D3D11_TEXTURE2D_DESC desc;
			texture2d->GetDesc(&desc);
			tDimensions.x = static_cast<float>(desc.Width);
			tDimensions.y = static_cast<float>(desc.Height);
		}
		ReleaseCOM(texture2d);
		ReleaseCOM(res);
	}

	ID3D11ShaderResourceView* m_pTexture;
	std::string tName;
	std::string tPath;
	Vector2 tDimensions;
};

