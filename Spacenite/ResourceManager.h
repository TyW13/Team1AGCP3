#pragma once
#include <vector>
#include <map>
#include <string>
#include "Texture.h"
#include "newGameObject.h"
#include "PlayerCharacter.h"
#include "Map.h"

using namespace DirectX;

class Texture;
class GameObject;
class PlayerCharacter;
//class TileSet;
//class Layers;

class ResourceManager
{
	// Commented out copy and move constructors and assignment operators

	//ResourceManager(const ResourceManager& other)												// Copy constructor
	//{
	//	this->m_Textures = other.m_Textures;
	//	this->m_gObjects = other.m_gObjects;
	//	this->pTex = other.pTex;
	//	this->pObj = other.pObj;
	//}
	//
	//ResourceManager& operator=(const ResourceManager& rhs)
	//{
	//	if (this != &rhs)
	//	{
	//		this->m_Textures = rhs.m_Textures;
	//		this->m_gObjects = rhs.m_gObjects;
	//
	//		if (this->pTex)
	//		{
	//			delete[] this->pTex;
	//		}
	//		if (this->pObj)
	//		{
	//			delete[] this->pObj;
	//		}
	//
	//		this->pTex = rhs.pTex;
	//		this->pObj = rhs.pObj;
	//	}
	//}
	//
	//ResourceManager(ResourceManager&& other)
	//{
	//	this->m_Textures = other.m_Textures;
	//	this->m_gObjects = other.m_gObjects;
	//	this->pTex = other.pTex;
	//	this->pObj = other.pObj;
	//
	//	other.pTex = nullptr;
	//	other.pObj = nullptr;
	//}
	//
	//ResourceManager& operator=(ResourceManager&& rhs)											// Move assignment
	//{
	//	// Self assignment check
	//	if (this != &rhs)
	//	{
	//		this->m_Textures = rhs.m_Textures;
	//		this->m_gObjects = rhs.m_gObjects;
	//
	//		if (this->pTex)
	//		{
	//			delete this->pTex;
	//		}
	//		if (this->pObj)
	//		{
	//			delete this->pObj;
	//		}
	//
	//		// Deleting the old data
	//		this->pObj = rhs.pObj;
	//		rhs.pObj = nullptr;
	//		this->pTex = rhs.pTex;
	//		rhs.pTex = nullptr;
	//	}
	//
	//	return *this;
	//}

public:
	ResourceManager(MyD3D& d3d){}

	void Init(ID3D11Device& pDevice, MyD3D& d3d);
	void Update(float dTime);
	void Render(SpriteBatch& batch);
	void Terminate();
	void CreateTexture(ID3D11Device& pDevice, const std::string &fPath);
	void AddGameObject(MyD3D& d3d);
	Texture* GetTexture(const std::string& tName);
	std::string SetTexName(std::string path);
	void LoadJSON();
	void LoadTiles(TileSet tileset, Layers layer);
private:
	std::map<std::string, Texture*> m_Textures;
	std::vector<GameObject*> m_gObjects;
};