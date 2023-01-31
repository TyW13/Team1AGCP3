#include "D3D.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "newGameObject.h"

void ResourceManager::Init(ID3D11Device* pDevice, MyD3D& d3d)
{
	AddTexture(pDevice, L"data/textures/testtexture.dds");
	AddGameObject(d3d);
}

void ResourceManager::Update()
{

}

void ResourceManager::Terminate()
{

}

void ResourceManager::AddTexture(ID3D11Device* pDevice, const std::string& fPath)
{
	Texture nTexture(pDevice, fPath);
	m_Textures[nTexture.GetName()] = &nTexture;
}

void ResourceManager::AddGameObject(MyD3D& d3d)
{
	GameObject nObject(d3d);
	nObject.Init();
	m_gObjects.push_back(&nObject);
}

// Get function for given texture using its std::string name used in the std::map
Texture& ResourceManager::GetTexture(const std::string& tName)
{
	// If given texture is not found in m_texture map, print error
	if (m_Textures.find(tName) != m_Textures.end())
	{
		return *m_Textures.at(tName);

	}
	else 
	{
		WDBOUT("ERROR: CANT FIND TEXTURE");
	}
}