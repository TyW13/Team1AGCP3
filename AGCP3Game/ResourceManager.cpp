#define NOMINMAX

#include <cstdio>
#include "ResourceManager.h"
#include "Tile.h"
#include "Player.h"

using namespace DirectX;

void ResourceManager::Init(DeviceManager* dManager)
{
	LoadTexturesFromFile(dManager);
	LoadLevelsFromFile();

	ReloadMap(dManager, 0);
}

void ResourceManager::Update(DeviceManager* dManager, float dTime)
{
	if (m_Objects.size() > 0)
	{
		for (int i = 0; i < m_Objects.size(); ++i)
		{
			m_Objects[i].get()->Update(dManager, this, dTime);
		}
	}
}

void ResourceManager::Render(DeviceManager* dManager)
{
	if (m_Objects.size() > 0)
	{
		for (int i = 0; i < m_Objects.size(); ++i)
		{
			m_Objects[i].get()->Render(dManager, *this);
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
	 
	UnloadZone();
}

void ResourceManager::LoadTexturesFromFile(DeviceManager* dManager)
{
	FILE* gTexturesFile;
	errno_t texturesStatus = fopen_s(&gTexturesFile, "Data/GameTextures.json", "rb");
	if (texturesStatus != 0)
	{
		printf("ERROR: Could not open file!");
		assert(false);
	}
	char readBuffer[4096];
	rapidjson::FileReadStream is(gTexturesFile, readBuffer, sizeof(readBuffer));
	rapidjson::Document texturesDoc;
	texturesDoc.ParseStream(is);
	fclose(gTexturesFile);

	Value::Array texturesArray = texturesDoc["textures"].GetArray();
	for (int i = 0; i < texturesArray.Size(); i++)
	{
		std::string fileStr = filePath + (texturesArray[i].GetString());																				// To convert from std::string to std::wstring
		std::wstring file(fileStr.begin(), fileStr.end());
		texture tex;

		dManager->GetResourceUpload()->Begin();																										// Descriptor and textures in GameTextures.json array must
																																					// be in the same order
		DX::ThrowIfFailed(
			DirectX::CreateDDSTextureFromFile(dManager->GetDevice(), *dManager->GetResourceUpload(), file.c_str(),
				&tex.resource));

		DirectX::CreateShaderResourceView(dManager->GetDevice(), tex.resource,
			dManager->GetResourceDescriptors()->GetCpuHandle(i));

		auto uploadResourcesFinished = dManager->GetResourceUpload()->End(
			dManager->GetDeviceResources()->GetCommandQueue());
		uploadResourcesFinished.wait();

		tex.id = i;
		tex.texPath = file;
		m_Textures.push_back(tex);
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

void ResourceManager::LoadCurrentMap(DeviceManager* dManager)
{
	ReloadMap(dManager, currentMapNum);
}

void ResourceManager::LoadNextMap(DeviceManager* dManager)
{
	if (currentMapNum < m_Levels.size() - 1)
	{
		currentMapNum++;
		ReloadMap(dManager, currentMapNum);
	}
}

void ResourceManager::LoadPreviousMap(DeviceManager* dManager)
{
	if (currentMapNum > 0)
	{
		currentMapNum--;
		ReloadMap(dManager, currentMapNum);
	}
}

// Function to eventually load in .json files which will be used to create gameobjects and tiles for the map
void ResourceManager::ReloadMap(DeviceManager* dManager, int mapNum)
{
	SetCurrentMap(mapNum);

	LoadZoneInfo(dManager, GetCurrentMap()->GetCurrentZoneNum());								// Calls the LoadZoneInfo function of currentmap, giving the currentzone we want to load
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
	if (m_Objects.size() > 0)
	{
		for (int i = m_Objects.size() - 1; i >= 0; --i)
		{
			m_Objects.pop_back();
		}
	}
}

void ResourceManager::LoadZoneInfo(DeviceManager* dManager, int zoneNum)
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

	DirectX::SimpleMath::Vector2 objScale = { 6,6 };
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

			float tileXPos = xPos * GetCurrentMap()->getTileWidth() * objScale.x;						// Tile object x and y position on screen
			float tileYPos = yPos * GetCurrentMap()->getTileWidth() * objScale.y;

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
				std::shared_ptr<Tile> tile = std::make_shared<Tile>(dManager, m_Textures[0].id, DirectX::SimpleMath::Vector2(tileXPos, tileYPos), objScale, true, DirectX::SimpleMath::Vector2(GetCurrentMap()->getTileWidth(), GetCurrentMap()->getTileHeight()), objType, true, tileRect);
				m_Objects.push_back(tile);
			}
			else if (objType == "Respawner")
			{
				std::shared_ptr<Tile> playerSpawner = std::make_shared<Tile>(dManager, m_Textures[0].id, DirectX::SimpleMath::Vector2(tileXPos, tileYPos), objScale, true, DirectX::SimpleMath::Vector2(GetCurrentMap()->getTileWidth(), GetCurrentMap()->getTileHeight()), objType, true, tileRect);				// Creating and pushing tile objects to m_Tiles vector
				m_Objects.push_back(playerSpawner);

				DirectX::SimpleMath::Vector2 playerSize{ 6,16 };
				float collisionOffset = 1.0f;
				RECT playerRect;
				playerRect.left = 0;
				playerRect.top = 0;
				playerRect.right = 6;
				playerRect.bottom = 16;
				int newPlayerYPos = (tileYPos + collisionHeight * objScale.y) - playerSize.y * objScale.y - collisionOffset;
				std::shared_ptr<Player> player = std::make_shared<Player>(dManager, m_Textures[1].id, DirectX::SimpleMath::Vector2(tileXPos, newPlayerYPos), objScale, true, playerSize, "Player", true, playerRect);
				m_Objects.push_back(player);
			}
			else if (objType == "Damageable")
			{
				std::shared_ptr<Tile> damageable = std::make_shared<Tile>(dManager, m_Textures[0].id, DirectX::SimpleMath::Vector2(tileXPos, tileYPos), objScale, true, DirectX::SimpleMath::Vector2(GetCurrentMap()->getTileWidth(), GetCurrentMap()->getTileHeight()), objType, true, tileRect);				// Creating and pushing tile objects to m_Tiles vector
				m_Objects.push_back(damageable);
			}
			else if (objType == "NextZone")
			{
				std::shared_ptr<Tile> nextZone = std::make_shared<Tile>(dManager, m_Textures[0].id, DirectX::SimpleMath::Vector2(tileXPos, tileYPos), objScale, true, DirectX::SimpleMath::Vector2(GetCurrentMap()->getTileWidth(), GetCurrentMap()->getTileHeight()), objType, true, tileRect);
				m_Objects.push_back(nextZone);
			}
		}
	}
}

void ResourceManager::LoadNextZone(DeviceManager* dManager)
{
	int zoneOffset = 1;
	if (GetCurrentMap()->GetCurrentZoneNum() < GetCurrentMap()->GetLayers().size() - 1)							// Check to see if incrementing zone num will go over max zones or not
	{
		GetCurrentMap()->SetCurrentZoneNum(GetCurrentMap()->GetCurrentZoneNum() + zoneOffset);
		LoadZoneInfo(dManager, GetCurrentMap()->GetCurrentZoneNum());
	}
}

void ResourceManager::LoadPreviousZone(DeviceManager* dManager)
{
	int zoneOffset = 1;
	if (GetCurrentMap()->GetCurrentZoneNum() > 0)
	{
		GetCurrentMap()->SetCurrentZoneNum(GetCurrentMap()->GetCurrentZoneNum() - zoneOffset);
		LoadZoneInfo(dManager, GetCurrentMap()->GetCurrentZoneNum());
	}
}

// TODO - Call Save function with button on UI and call Load function on rManager Init
void ResourceManager::SavePlayerData()
{
	playerDataFile.open("data/playerData.txt");													// Opens playerData text file 
	playerDataFile << GetCurrentMapNum();														// Writes players current map number to file
	playerDataFile << "\n";
	playerDataFile << GetCurrentMap()->GetCurrentZoneNum();										// Writes players current zone number to file
	playerDataFile.close();																		// Closes playerData text file
}

void ResourceManager::LoadPlayerData()
{
	playerDataFile.open("data/playerData.txt");													// Opens playerData text file 

	std::string line;																			// String to temporarily store data being pulled from text file

	std::getline(playerDataFile, line);															// Grabs first number from text file which relates to the map number
	SetCurrentMap(stoi(line));																	// Converts map number from string to int and sets it 

	std::getline(playerDataFile, line);															// Grabs second number from text file which relates to the zone number
	GetCurrentMap()->SetCurrentZoneNum(stoi(line));												// Converts zone number from string to int and sets it 

	playerDataFile.close();																		// Closes playerData text file
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
