#include <cstdio>
#include "ResourceManager.h"
#include "Map.h"
#include "Tile.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;
using namespace DirectX;


void ResourceManager::Init(ID3D11Device& pDevice, MyD3D& d3d)
{
	// Will eventually not need to hard code in file, should read from .json file
	CreateTexture(pDevice, "testTexture.dds");
	CreateTexture(pDevice, "ship.dds");
	CreateTexture(pDevice, "test_sheet2.dds");
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
	//PlayerCharacter* obj = new PlayerCharacter(d3d, GetTexture("ship"), Vector2(20, 20), Vector2(0.1, 0.1), true);							// Manually creating new player object

	//m_gObjects.emplace_back(obj);

	for (int i = 0; i < tileRects.size(); i++)
	{
		//Tile newTile(d3d, GetTexture("test_sheet"), tilePositions[i], Vector2(6, 4), true, tileRects[i]);
		Tile* newTile = new Tile(d3d, GetTexture("test_sheet2"), tilePositions[i], Vector2(1, 1), true, tileRects[i], i);
		m_gObjects.emplace_back(newTile);
	}
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
	Map testMap("data/test_level_jump1.json");

	// IMPORT CODE TO CONVERT TSX FILE TO JSON FILE
	std::string newSource = "data/" + testMap.getSource().substr(0, testMap.getSource().size() - 4) + ".json";


	FILE* f = fopen(newSource.c_str(), "rb");		// opens json file 

	if (!f)
	{
		printf("didnt work");
		assert(false);
	}

	char readBuffer[4096];
	FileReadStream is(f, readBuffer, sizeof(readBuffer));

	Document tilesetDoc;
	tilesetDoc.ParseStream(is);			// parses json file 

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

	LoadTileSet(testMap);
}

void ResourceManager::LoadTileSet(Map map)
{
	std::vector<int> data = map.getData();
	for (size_t i = 0; i < data.size(); i++)
	{
		//if (data[i] != 0)
		//{
			size_t columns = ts_Columns;
			size_t val = data[i] - map.getFirstgid();								// Value of tile in tileset starting from 0 -> 13

			size_t x = val % columns;												// Position of tile on the tile map, (0,0) is top left going down and to the right
			size_t y = floor(val / columns);										// Floor rounds down (returns biggest int thats lower than original value)
			
			size_t xPos = i % map.getWidth();										//
			size_t yPos = floor(i / map.getWidth());								//

			float tileXPos = xPos * ts_Tilewidth * 6;									// Tile object x and y position on screen
			float tileYPos = yPos * ts_Tileheight * 6;

			tilePositions.emplace_back(Vector2(tileXPos, tileYPos));

			float x1 = x * ts_Tilewidth;											// Pixel coordinates on tileset image, each corner of a tile square (like int rect from sfml)
			float x2 = (x + 1) * ts_Tilewidth;
			float y1 = y * ts_Tileheight;
			float y2 = (y + 1) * ts_Tileheight;

			// CONSTRUCT TILE WITH POSITIONS or JUST CHECK POSITIONS FOR FUTUre

			RECTF tileRect;
			tileRect.left = x1;
			tileRect.right = x2;
			tileRect.top = y1;
			tileRect.bottom = y2;

			tileRects.emplace_back(tileRect);
		//}
	}
}

Map::Map(const char* filePath)
{
	FILE* fp = fopen(filePath, "rb");		// opens json file 

	char readBuffer[4096];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document document;
	document.ParseStream(is);			// parses json file 

	fclose(fp);		// closes json once it has been read 

	// Stores all the data from the json file in the respective variables from the class

	height = document["height"].GetInt();
	infinite = document["infinite"].GetBool();
	
	Value::Array layersArray = document["layers"].GetArray();
	//rapidjson::GenericObject layersObj = layersArray[0]

	GenericArray layersA = document["layers"].GetArray();
	//GenericObject test = layersA.begin()->GetObject();
	//Value:: layerValue = layersA[0];					////// FIX THIS LOOP STUFF ////

	for (size_t i = 0; i < layersA.Capacity(); i++)
	{
		if (layersA[0].HasMember("data"))
		{
			for (auto& v : layersA[0]["data"].GetArray())
			{
				data.push_back(v.GetInt());
			}
		}
	}

	
	name = layersA[0]["name"].GetString();
	id = layersA[0]["id"].GetInt();
	opacity = layersA[0]["opacity"].GetInt();
	layersType = layersA[0]["type"].GetString();
	visible = layersA[0]["visible"].GetBool();
	x = layersA[0]["x"].GetInt();
	y = layersA[0]["y"].GetInt();

	nextobjectid = document["nextobjectid"].GetInt();
	orientation = document["orientation"].GetString();
	renderorder = document["renderorder"].GetString();
	tileheight = document["tileheight"].GetInt();

	Value::Array tilesets = document["tilesets"].GetArray();

	firstgid = tilesets[0]["firstgid"].GetInt();
	source = tilesets[0]["source"].GetString();

	tilewidth = document["tilewidth"].GetInt();
	type = document["type"].GetString();
	width = document["width"].GetInt();
}
