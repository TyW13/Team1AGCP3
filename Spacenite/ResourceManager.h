#pragma once
#include <vector>
#include <map>
#include <string>
#include "D3DUtil.h"

class Texture;

class ResourceManager
{
public:
	void Init(ID3D11Device* pDevice);
	void Update();
	void Terminate();
	void AddTexture(ID3D11Device* pDevice, std::wstring &fPath);
	Texture& GetTexture(const std::wstring& tName);
private:
	std::map<std::wstring, Texture> m_Textures;
};

