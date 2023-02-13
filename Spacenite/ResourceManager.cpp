#include "D3D.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include <cstdio>
#include "ResourceManager.h"
#include "PlayerCharacter.h"

#include "rapidjson/filereadstream.h"
#include "Map.h"

void ResourceManager::Init(ID3D11Device& pDevice, MyD3D& d3d)
{
	// Will eventually not need to hard code in file, should read from .json file
	CreateTexture(pDevice, "testTexture.dds");
	CreateTexture(pDevice, "ProgChar.dds");
}

void ResourceManager::Update(float dTime)
{
	if (m_gObjects.size() > 0)
	{
		for (GameObject* currentObj : m_gObjects)
		{
			if (currentObj->GetActive())
			{
				currentObj->Update(dTime);
			}
		}
	}
}

void ResourceManager::Render(SpriteBatch& batch)
{
	if (m_gObjects.size() > 0)
	{
		for (GameObject* currentObj : m_gObjects)
		{
			if (currentObj->GetActive())
			{
				currentObj->GetSprite().Draw(batch);
			}
		}
	}
}

void ResourceManager::Terminate()
{
	for (const auto& tex : m_Textures)
	{
		delete tex.second;
	}
	 
	for (const auto& obj : m_gObjects)
	{
		delete obj;
	}
}

// Creates a new texture object to add to the std::map
void ResourceManager::CreateTexture(ID3D11Device& pDevice, const std::string& fPath)
{
	Texture* t = new Texture(fPath);

	m_Textures.emplace(t->GetName(), t);
}

// Function to add gameobjects to resource manager. Will eventually work with json file to mass import objects
void ResourceManager::AddGameObject(MyD3D& d3d)
{
	PlayerCharacter* obj = new PlayerCharacter(d3d, GetTexture("ProgChar"), Vector2(0.5, 0.5), true);							// Manually creating new player object

	m_gObjects.emplace_back(obj);
}

// Get function for given texture using its std::string name used in the std::map
Texture* ResourceManager::GetTexture(const std::string& tName)
{
	// If given texture is not found in m_texture map, print error
	if (m_Textures.find(tName) != m_Textures.end())
	{
		return m_Textures.at(tName);
	}
	else 
	{
		WDBOUT("ERROR: CANT FIND TEXTURE");
	}
}

// Reduces the given texture path to just the image name to give it a more appropriate name
std::string ResourceManager::SetTexName(std::string path)
{
	// path is data/(.dds)
	path.substr(0, 1);
	auto itr = path.find_last_of(".");
	std::string noSuff = path.substr(0, itr);
	auto lastSlash = noSuff.find_last_of("/");
	std::string noPath = noSuff.substr(lastSlash + 1, noSuff.length() - lastSlash);
	return noPath;
}

// Function to eventually load in .json files which will be used to create gameobjects and tiles for the map
void ResourceManager::LoadJSON()
{
	Map testMap;
}
