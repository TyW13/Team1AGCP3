#include <cstdio>
#include "ResourceManager.h"
#include "Map.h"

using namespace rapidjson;

void ResourceManager::Init(ID3D11Device& pDevice, MyD3D& d3d)
{
	// Will eventually not need to hard code in file, should read from .json file
	CreateTexture(pDevice, "testTexture.dds");
	CreateTexture(pDevice, "ship.dds");
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
	PlayerCharacter* obj = new PlayerCharacter(d3d, GetTexture("ship"), Vector2(20, 20), Vector2(0.5, 0.5), true);							// Manually creating new player object

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
void ResourceManager::LoadJSON(Map& map)
{
	//Map testMap;
	FILE* f = fopen("data/TSTestingLevel0.json", "rb");		// opens json file 

	if (!f)
	{
		printf("didnt work");
	}

	char readBuffer[10000];
	FileReadStream is(f, readBuffer, sizeof(readBuffer));

	Document tilesetDoc;
	tilesetDoc.ParseStream(is);			// parses json file 

	fclose(f);

	TileSet tileSet;
	tileSet.Init(tilesetDoc);
}

void ResourceManager::LoadTileSet(TileSet& tileSet, Map map)
{
	std::vector<int> data = map.getData();
	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i] != 0)
		{
			size_t columns = tileSet.getColumns();
			size_t val = data[i] - tileSet.getFirstGid();

			size_t x = val % columns;
			size_t y = floor(val / columns);										// Floor rounds down (returns biggest int thats bigger than original value)

			size_t xPos = i % map.getWidth();
			size_t yPos = floor(i / map.getWidth());

			float tileXPos = xPos * tileSet.getTileWidth();
			float tileYPos = yPos * tileSet.getTileHeight();

			// CONSTRUCT TILE WITH POSITIONS or JUST CHECK POSITIONS FOR FUTUre
		}
	}
}
