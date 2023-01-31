#pragma once
#include <vector>
#include <map>
#include <string>
#include "D3DUtil.h"

class Texture;
class GameObject;

class ResourceManager
{
public:
	void Init(ID3D11Device* pDevice, MyD3D& d3d);
	void Update();
	void Terminate();
	void AddTexture(ID3D11Device* pDevice, const std::string &fPath);
	void AddGameObject(MyD3D& d3d);
	Texture& GetTexture(const std::string& tName);
private:
	std::map<std::string, std::string> m_Textures;
	std::vector<GameObject*> m_gObjects;
};