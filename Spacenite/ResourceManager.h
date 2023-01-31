#pragma once
#include <vector>
#include <map>
#include <string>
#include "D3DUtil.h"
#include "Sprite.h"

class Texture;
class GameObject;

using namespace DirectX;

class ResourceManager
{
public:
	void Init(ID3D11Device& pDevice, MyD3D& d3d);
	void Render(SpriteBatch& batch);
	void Terminate();
	void CreateTexture(ID3D11Device& pDevice, const std::string &fPath);
	void AddGameObject(GameObject* newObject);
	Texture& GetTexture(const std::string& tName);
private:
	std::map<std::string, Texture*> m_Textures;
	std::vector<GameObject*> m_gObjects;
};