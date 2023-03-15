#include <cstdio>
#include "ResourceManager.h"
#include "PlayerCharacter.h"
#include "Tile.h"
#include "Game.h"


using namespace DirectX;


void ResourceManager::Init(MyD3D& d3d)
{
	LoadTexturesFromFile();
	LoadLevelsFromFile();

	ReloadMap(d3d, 0);
}

void ResourceManager::Update(MyD3D& d3d, float dTime)
{
	if (m_gObjects.size() > 0)
	{
		for (GameObject* currentObj : m_gObjects)
		{
			if (currentObj->GetActive())
			{
				currentObj->Update(d3d, dTime, *this);
			}
		}
	}

	// TEMPORARY FORCED MAP AND ZONE TRANSITIONS
	if (Game::sMKIn.IsPressed(VK_RIGHT) == true)
	{
		LoadNextMap(d3d);
	}
	if (Game::sMKIn.IsPressed(VK_LEFT) == true)
	{
		LoadPreviousMap(d3d);
	}
	if (Game::sMKIn.IsPressed(VK_DOWN) == true)
	{
		LoadNextZone(d3d);
	}
	if (Game::sMKIn.IsPressed(VK_UP) == true)
	{
		LoadPreviousZone(d3d);
	}
}

void ResourceManager::Render(SpriteBatch& batch)
{
	if (m_Tiles.size() > 0)
	{
		for (Tile* currentTile : m_Tiles)
		{
			currentTile->GetSprite().Draw(batch);
		}
	}
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
	for (Map* map : m_Levels)
	{
		delete map;
		map = nullptr;
	}
	for (const auto& tex : m_Textures)
	{
		delete tex.second;
	}
	 
	for (const auto& obj : m_gObjects)
	{
		delete obj;
	}
	for (Tile* tile : m_Tiles)
	{
		delete tile;
		tile = nullptr;
	}
}

void ResourceManager::LoadTexturesFromFile()
{
	FILE* gLevelsFile = fopen("data/GameTextures.json", "rb");
	char readBuffer[4096];
	FileReadStream is(gLevelsFile, readBuffer, sizeof(readBuffer));
	Document texDoc;
	texDoc.ParseStream(is);

	Value::Array texturesArray = texDoc["textures"].GetArray();
	for (int i = 0; i < texturesArray.Size(); i++)
	{
		std::string file = texturesArray[i].GetString();
		Texture* t = new Texture((file).c_str());
		m_Textures.emplace(t->GetName(), t);
	}
}

// Goes through levels json file to add all needed level names to vector
void ResourceManager::LoadLevelsFromFile()
{
	FILE* gLevelsFile = fopen("data/GameLevels.json", "rb");
	char readBuffer[4096];
	FileReadStream is(gLevelsFile, readBuffer, sizeof(readBuffer));
	Document levelsDoc;
	levelsDoc.ParseStream(is);

	Value::Array levelsArray = levelsDoc["levels"].GetArray();
	for (int i = 0; i < levelsArray.Size(); i++)
	{
		std::string file = levelsArray[i].GetString();
		Map* newMap = new Map(("data/" + file).c_str());
		m_Levels.emplace_back(newMap);
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

void ResourceManager::SetCurrentMap(int _currentMapNum)
{
	currentMapNum = _currentMapNum;
}

void ResourceManager::LoadCurrentMap(MyD3D& d3d)
{
	ReloadMap(d3d, currentMapNum);
}

void ResourceManager::LoadNextMap(MyD3D& d3d)
{
	if (currentMapNum < m_Levels.size() - 1)
	{
		currentMapNum++;
		ReloadMap(d3d, currentMapNum);
	}
}

void ResourceManager::LoadPreviousMap(MyD3D& d3d)
{
	if (currentMapNum > 0)
	{
		currentMapNum--;
		ReloadMap(d3d, currentMapNum);
	}
}

// Function to eventually load in .json files which will be used to create gameobjects and tiles for the map
void ResourceManager::ReloadMap(MyD3D& d3d, int mapNum)
{
	SetCurrentMap(mapNum);

	LoadZoneInfo(d3d, GetCurrentMap()->GetCurrentZoneNum());								// Calls the LoadZoneInfo function of currentmap, giving the currentzone we want to load
}

Map::Map(const char* filePath)
{
	currentZoneNum = 0;

	FILE* fp = fopen(filePath, "rb");		// opens json file 

	char readBuffer[4096];
	rapidjson::FileReadStream mapStream(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document document;
	document.ParseStream(mapStream);		// parses json file 

	fclose(fp);		// closes json once it has been read 

	// Stores all the data from the json file in the respective variables from the class
	height = document["height"].GetInt();
	infinite = document["infinite"].GetBool();

	GenericArray layersA = document["layers"].GetArray();
	for (size_t i = 0; i < layersA.Capacity(); i++)
	{
		Layer layer(layersA[i]);
		layers.push_back(layer);
	}

	nextobjectid = document["nextobjectid"].GetInt();
	orientation = document["orientation"].GetString();
	renderorder = document["renderorder"].GetString();
	tileheight = document["tileheight"].GetInt();
	tilewidth = document["tilewidth"].GetInt();
	type = document["type"].GetString();
	width = document["width"].GetInt();

	Value::Array tilesets = document["tilesets"].GetArray();
	ts_firstgid = tilesets[0]["firstgid"].GetInt();
	ts_source = tilesets[0]["source"].GetString();

	// IMPORT CODE TO CONVERT TILESET TSX FILE TO JSON FILE
	std::string newSource = "data/" + ts_source.substr(0, ts_source.size() - 4) + ".json";

	FILE* f = fopen(newSource.c_str(), "rb");		// opens json file 

	if (!f)
	{
		assert(false);
	}

	readBuffer[4096];
	FileReadStream tsStream(f, readBuffer, sizeof(readBuffer));

	Document tilesetDoc;
	tilesetDoc.ParseStream(tsStream);			// parses json file 

	fclose(f);

	ts_Columns = tilesetDoc["columns"].GetInt();
	ts_Image = tilesetDoc["image"].GetString();
	ts_ImageHeight = tilesetDoc["imageheight"].GetInt();
	ts_ImageWidth = tilesetDoc["imagewidth"].GetInt();
	ts_Margin = tilesetDoc["margin"].GetInt();
	ts_Name = tilesetDoc["name"].GetString();
	ts_Spacing = tilesetDoc["spacing"].GetInt();
	ts_Tilecount = tilesetDoc["tilecount"].GetInt();
	ts_Tileheight = tilesetDoc["tileheight"].GetInt();
	ts_Tilewidth = tilesetDoc["tilewidth"].GetInt();
	ts_Type = tilesetDoc["type"].GetString();
}

void ResourceManager::LoadNextZone(MyD3D& d3d)
{
	if (GetCurrentMap()->GetCurrentZoneNum() < GetCurrentMap()->GetLayers().size() - 1)							// Check to see if incrementing zone num will go over max zones or not
	{
		GetCurrentMap()->SetCurrentZoneNum(GetCurrentMap()->GetCurrentZoneNum() + 1);
		LoadZoneInfo(d3d, GetCurrentMap()->GetCurrentZoneNum());
	}
}

void ResourceManager::LoadPreviousZone(MyD3D& d3d)
{
	if (GetCurrentMap()->GetCurrentZoneNum() > 0)
	{
		GetCurrentMap()->SetCurrentZoneNum(GetCurrentMap()->GetCurrentZoneNum() - 1);
		LoadZoneInfo(d3d, GetCurrentMap()->GetCurrentZoneNum());
	}
}

void ResourceManager::UnloadZone()
{
	if (m_gObjects.size() > 0)
	{
		for (int i = m_gObjects.size() - 1; i >= 0; i--)
		{
			delete m_gObjects[i];
			m_gObjects[i] = nullptr;
			m_gObjects.pop_back();
		}
	}
}

void ResourceManager::LoadZoneInfo(MyD3D& d3d, int zoneNum)
{
	UnloadZone();

	FILE* fp = fopen("data/TSTestingLevel0.json", "rb");		// opening
	char readBuffer[4096];
	rapidjson::FileReadStream mapStream(fp, readBuffer, sizeof(readBuffer));
	Document tilesetDoc;
	tilesetDoc.ParseStream(mapStream);
	fclose(fp);

	GetCurrentMap()->SetCurrentZoneNum(zoneNum);

	std::vector<int> data = GetCurrentMap()->GetCurrentZone().GetData();						// Initializing new vector<int> with current zones datavector (storing tile data)

	int collisionWidth = 0;
	int collisionHeight = 0;
	std::string objType;
	bool isCollidable;

	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i] != 0)
		{
			size_t columns = GetCurrentMap()->GetColumns();
			size_t val = data[i] - GetCurrentMap()->getFirstgid();								// Value of tile in tileset starting from 0 -> 13

			size_t x = val % columns;															// Position of tile on the tile map, (0,0) is top left going down and to the right
			size_t y = floor(val / columns);													// Floor rounds down (returns biggest int thats lower than original value)

			size_t xPos = i % GetCurrentMap()->getWidth();										//
			size_t yPos = floor(i / GetCurrentMap()->getWidth());								//

			float tileXPos = xPos * GetCurrentMap()->getTileWidth() * 6;						// Tile object x and y position on screen
			float tileYPos = yPos * GetCurrentMap()->getTileWidth() * 6;;

			float x1 = x * GetCurrentMap()->getTileWidth();										// Pixel coordinates on tileset image, each corner of a tile square (like int rect from sfml)
			float x2 = (x + 1) * GetCurrentMap()->getTileWidth();
			float y1 = y * GetCurrentMap()->getTileHeight();
			float y2 = (y + 1) * GetCurrentMap()->getTileHeight();

			RECTF tileRect;																		// Creating new RECT to pass to tile object constructor
			tileRect.left = x1;
			tileRect.right = x2;
			tileRect.top = y1;
			tileRect.bottom = y2;

			collisionWidth = tilesetDoc["tiles"].GetArray()[val]["objectgroup"].GetObj()["objects"].GetArray()[0]["width"].GetInt();			// Get collision bounds width and height from tileset json
			collisionHeight = tilesetDoc["tiles"].GetArray()[val]["objectgroup"].GetObj()["objects"].GetArray()[0]["height"].GetInt();
			objType = tilesetDoc["tiles"].GetArray()[val]["objectgroup"].GetObj()["objects"].GetArray()[0]["class"].GetString();
			isCollidable = tilesetDoc["tiles"].GetArray()[val]["properties"].GetArray()[0]["value"].GetBool();

			if (objType == "Tile")
			{ 
				Tile* newTile = new Tile(d3d, GetTexture("test_sheet2"), Vector2(tileXPos, tileYPos), Vector2(6, 6), true, Vector2(GetCurrentMap()->getTileWidth(), GetCurrentMap()->getTileHeight()), objType, true, tileRect);				// Creating and pushing tile objects to m_Tiles vector
				m_gObjects.emplace_back(newTile);
			}
			else if (objType == "Damageable")
			{
				Tile* newDamageable = new Tile(d3d, GetTexture("test_sheet2"), Vector2(tileXPos, tileYPos), Vector2(6, 6), true, Vector2(GetCurrentMap()->getTileWidth(), GetCurrentMap()->getTileHeight()), objType, true, tileRect);				// Creating and pushing tile objects to m_Tiles vector
				m_gObjects.emplace_back(newDamageable);
			}
			else if (objType == "ReloadGem")	// NOT SET UP COMPLETELY YET 
			{
				Tile* newReloadGem = new Tile(d3d, GetTexture("test_sheet2"), Vector2(tileXPos, tileYPos), Vector2(6, 6), true, Vector2(GetCurrentMap()->getTileWidth(), GetCurrentMap()->getTileHeight()), objType, true, tileRect);
				m_gObjects.emplace_back(newReloadGem);
			}
			else if (objType == "Respawner")
			{
				currentSpawner = new Respawner(d3d, GetTexture("test_sheet2"), Vector2(tileXPos, tileYPos), Vector2(6, 6), true, Vector2(GetCurrentMap()->getTileWidth(), GetCurrentMap()->getTileHeight()), objType, true, tileRect);
				m_gObjects.emplace_back(currentSpawner);

				Vector2 playerSize{ 6,16 };
				Vector2 playerScale{ 6,6 };
				float collisionOffset = 1.0f;
				PlayerCharacter* player = new PlayerCharacter(d3d, GetTexture("newtest_chara_walk"), Vector2(tileXPos, tileYPos - (playerSize.y * playerScale.y + collisionOffset)), Vector2(6, 6), true, Vector2(6, 16), "Player", true);
				m_gObjects.emplace(m_gObjects.begin(), player);
				//if (m_gObjects[0]->GetObjectType() != "Player")
				//{
				//	PlayerCharacter* player = new PlayerCharacter(d3d, GetTexture("newtest_chara_walk"), Vector2(tileXPos, tileYPos - (playerSize.y * playerScale.y + collisionOffset)), Vector2(6, 6), true, Vector2(6, 16), "Player", true);
				//	m_gObjects.emplace(m_gObjects.begin(), player);
				//}
				//else if (m_gObjects[0]->GetObjectType() == "Player")
				//{
				//	m_gObjects[0]->GetSprite().SetPosition(Vector2(tileXPos, tileYPos - (playerSize.y * playerScale.y + collisionOffset)));
				//}
			}
		}
	}
}

Layer::Layer(Value& value)
{
	if (value.HasMember("data")) {
		for (auto& a : value["data"].GetArray()) {
			data.push_back(a.GetInt());
		}
	}

	if (value.HasMember("height")) {
		height = value["height"].GetInt();
	}

	id = value["id"].GetInt();

	if (value.HasMember("image")) {
		image = value["image"].GetString();
	}

	name = value["name"].GetString();
	opacity = value["opacity"].GetInt();
	layersType = value["type"].GetString();
	visible = value["visible"].GetBool();

	if (value.HasMember("width")) {
		width = value["width"].GetInt();
	}

	x = value["x"].GetInt();
	y = value["y"].GetInt();
}
