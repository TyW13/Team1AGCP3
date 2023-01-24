#include "ResourceManager.h"
#include "Texture.h"

void ResourceManager::Init(ID3D11Device* pDevice)
{
	AddTexture(pDevice, L"data/textures/testtexture.dds");

}

void ResourceManager::Update()
{

}

void ResourceManager::Terminate()
{

}

void ResourceManager::AddTexture(ID3D11Device* pDevice, const std::wstring& fPath)
{
	Texture newTexture(pDevice, fPath);
	m_Textures[newTexture.GetName()] = newTexture;
}

// Get function for given texture using its std::string name used in the std::map
Texture& ResourceManager::GetTexture(const std::wstring& tName)
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