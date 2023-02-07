#include "D3D.h"
#include "ResourceManager.h"
#include "PlayerCharacter.h"

void ResourceManager::Init(ID3D11Device& pDevice, MyD3D& d3d)
{
	CreateTexture(pDevice, "testTexture.dds");
	//AddTexture(pDevice, "data/textures/testtexture.dds");
	//AddGameObject(d3d);
}

void ResourceManager::Render(SpriteBatch& batch)
{
	for (PlayerCharacter* currentObj : m_gObjects)
	{
		if (currentObj->GetActive())
		{			  
			currentObj->GetSprite().Draw(batch);
		}
	}
}

void ResourceManager::Terminate()
{
	//for (const auto& tex : m_Textures)
	//{
	//	delete tex.second;
	//}
}

void ResourceManager::CreateTexture(ID3D11Device& pDevice, const std::string& fPath)
{
	pTex.Init(fPath);

	m_Textures.emplace(pTex.GetName(), pTex);
}

void ResourceManager::AddGameObject(MyD3D& d3d, GameObject newObject)
{
	PlayerCharacter* p = new PlayerCharacter(d3d, GetTexture("testTexture"), Vector2(1, 1), true);
	//pObj = &newObject;
	//GameObject tempObj = &p;
	m_gObjects.emplace_back(p);
}

// Get function for given texture using its std::string name used in the std::map
Texture ResourceManager::GetTexture(const std::string& tName)
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

std::string ResourceManager::SetTexName(std::string path)
{
	// path is data/textures/
	path.substr(0, 1);
	auto itr = path.find_last_of(".");
	std::string noSuff = path.substr(0, itr);
	auto lastSlash = noSuff.find_last_of("/");
	std::string noPath = noSuff.substr(lastSlash + 1, noSuff.length() - lastSlash);
	return noPath;
}