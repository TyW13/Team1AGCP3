#define NOMINMAX

#include <cstdio>
#include "ResourceManager.h"

using namespace DirectX;


void ResourceManager::Init(DeviceManager* d3d)
{
	LoadLevelsFromFile();

	ReloadMap(d3d, 0);
}

void ResourceManager::Render(DeviceManager* d3d)
{
	if (m_Tiles.size() > 0)
	{
		for (Tile* currentTile : m_Tiles)
		{
			currentTile->Render(d3d);
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
	 
	for (Tile* tile : m_Tiles)
	{
		delete tile;
		tile = nullptr;
	}
}

// Goes through levels json file to add all needed level names to vector
void ResourceManager::LoadLevelsFromFile()
{
	FILE* gLevelsFile;
	errno_t levelsStatus = fopen_s(&gLevelsFile, "Data/GameLevels.json", "rb");
	if (levelsStatus != 0)
	{
		printf("ERROR: Could not open file!");
		assert(false);
	}
	char readBuffer[4096];
	rapidjson::FileReadStream is(gLevelsFile, readBuffer, sizeof(readBuffer));
	rapidjson::Document levelsDoc;
	levelsDoc.ParseStream(is);
	fclose(gLevelsFile);

	Value::Array levelsArray = levelsDoc["levels"].GetArray();
	for (int i = 0; i < levelsArray.Size(); i++)
	{
		std::string file = levelsArray[i].GetString();
		Map* newMap = new Map(("Data/" + file).c_str());
		m_Levels.emplace_back(newMap);
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

// Function to eventually load in .json files which will be used to create gameobjects and tiles for the map
void ResourceManager::ReloadMap(DeviceManager* d3d, int mapNum)
{
	SetCurrentMap(mapNum);

	LoadZoneInfo(d3d, GetCurrentMap()->GetCurrentZoneNum());								// Calls the LoadZoneInfo function of currentmap, giving the currentzone we want to load
}

Map::Map(const char* filePath)
{
	currentZoneNum = 0;

	FILE* fp;
	errno_t mapStatus = fopen_s(&fp, filePath, "rb");		// opens json file 
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
	std::string newSource = "Data/" + ts_source.substr(0, ts_source.size() - 4) + ".json";

	FILE* f;
	errno_t tileSetStatus = fopen_s(&f, newSource.c_str(), "rb");		// opens json file 

	if (!f)
	{
		assert(false);
	}

	readBuffer[4096];
	rapidjson::FileReadStream tsStream(f, readBuffer, sizeof(readBuffer));

	rapidjson::Document tilesetDoc;
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

void ResourceManager::UnloadZone()
{
	if (m_Tiles.size() > 0)
	{
		for (int i = m_Tiles.size() - 1; i >= 0; i--)
		{
			delete m_Tiles[i];
			m_Tiles[i] = nullptr;
			m_Tiles.pop_back();
		}
	}
}

void ResourceManager::LoadZoneInfo(DeviceManager* d3d, int zoneNum)
{
	UnloadZone();

	FILE* fp;
	errno_t tileSetStatus = fopen_s(&fp, "Data/TSTestingLevel0.json", "rb");		// opening
	char readBuffer[4096];
	rapidjson::FileReadStream mapStream(fp, readBuffer, sizeof(readBuffer));
	rapidjson::Document tilesetDoc;
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

			RECT tileRect;																		// Creating new RECT to pass to tile object constructor
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
				Tile* newTile = new Tile(d3d, DirectX::SimpleMath::Vector2(tileXPos, tileYPos), DirectX::SimpleMath::Vector2(6, 6), true, DirectX::SimpleMath::Vector2(GetCurrentMap()->getTileWidth(), GetCurrentMap()->getTileHeight()), objType, true, tileRect);				// Creating and pushing tile objects to m_Tiles vector
				m_Tiles.emplace_back(newTile);
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
