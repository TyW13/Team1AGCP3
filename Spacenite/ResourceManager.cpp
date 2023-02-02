#include "D3D.h"
#include "ResourceManager.h"
#include "newGameObject.h"

void ResourceManager::Init(ID3D11Device& pDevice, MyD3D& d3d)
{
	CreateTexture(pDevice, "testTexture.dds");
	//AddTexture(pDevice, "data/textures/testtexture.dds");
	//AddGameObject(d3d);
}

void ResourceManager::Render(SpriteBatch& batch)
{
	for (GameObject* currentObj : m_gObjects)
	{
		if (currentObj->GetActive())
		{
			currentObj->GetSprite().Draw(batch);
		}
	}
}

void ResourceManager::Terminate()
{
	for (const auto& tex : m_Textures)
	{
		delete tex.second;
	}
}

void ResourceManager::CreateTexture(ID3D11Device& pDevice, const std::string& fPath)
{
	pTex = new Texture;
	pTex->Init(fPath);

	/*m_Textures[nTexture.GetName()] = &nTexture;
	m_Textures.insert({ nTexture.GetName(), &nTexture });*/
	m_Textures.emplace(pTex->GetName(), pTex);
}

void ResourceManager::AddGameObject(GameObject* newObject)
{
	pObj = newObject;
	//GameObject nObject(d3d);
	m_gObjects.push_back(pObj);
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