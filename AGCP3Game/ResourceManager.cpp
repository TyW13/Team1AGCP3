#define NOMINMAX

#include <cstdio>
#include "Tile.h"
#include "UIElement.h"
#include "StartButton.h"
#include "ExitButton.h"
#include "ResourceManager.h"

void ResourceManager::Init(DeviceManager* dManager)
{
	AddTitleScreenUI(dManager);
}

void ResourceManager::Update(DeviceManager* dManager, float dTime)
{
	switch (currentGameState)
	{
	case(EGameStates::TITLE):
		CheckTitleOnClickEvents(dManager);
		break;
	case(EGameStates::GAME):
		if (GetCurrentMap()->GetCurrentZone()->GetPlayer() != nullptr)
		{
			GetCurrentMap()->GetCurrentZone()->GetPlayer()->Update(dManager, this, dTime);
			GetCurrentMap()->GetCurrentZone()->GetShotgunObj()->Update(dManager, this, dTime);
		}
		else { assert(false); }
		break;
	case(EGameStates::CREDITS):
		// PLAN: IN THE FUTURE ADD CREDITS SCREEN SHOWING EVERYONES NAMES, WHICH THE PLAYER CAN EXIT TO TITLE SCREEN FROM
		break;
	}
}

void ResourceManager::Render(DeviceManager* dManager)
{
	switch (currentGameState)
	{
	case(EGameStates::TITLE):
		for (auto& element : m_TitleUIElements)
		{
			element.get()->Render(dManager);
		}
		break;
	case(EGameStates::GAME):
		if (GetCurrentMap()->GetCurrentZone()->GetTiles().size() > 0)
		{
			for (auto& currentObj : GetCurrentMap()->GetCurrentZone()->GetTiles())
			{
				if (currentObj.get()->GetActive())
				{
					currentObj.get()->Render(dManager);
				}
			}
		}
		if (GetCurrentMap()->GetCurrentZone()->GetPlayer() != nullptr && GetCurrentMap()->GetCurrentZone()->GetPlayer()->GetActive())
		{
			GetCurrentMap()->GetCurrentZone()->GetPlayer()->Render(dManager);
			GetCurrentMap()->GetCurrentZone()->GetShotgunObj()->Render(dManager);
		}
		break;
	case(EGameStates::CREDITS):
		// PLAN: SEE ResourceManager::Update
		break;
	}
}

void ResourceManager::AddTitleScreenUI(DeviceManager* dManager)
{
	m_TitleUIElements.push_back(std::make_shared<UIElement>(dManager, UIFolderPath + "UI_TitleMasterSheet.dds", DirectX::SimpleMath::Vector2(0, 0), DirectX::SimpleMath::Vector2(10, 10), DirectX::SimpleMath::Vector2(192, 108), RECT{ (0), (438), (192), (546) }));
	m_TitleUIElements.push_back(std::make_shared<StartButton>(dManager, UIFolderPath + "UI_TitleMasterSheet.dds", DirectX::SimpleMath::Vector2(674, 426), DirectX::SimpleMath::Vector2(1, 1), DirectX::SimpleMath::Vector2(572, 146), RECT{ (0), (146), (572), (292) }));
	m_TitleUIElements.push_back(std::make_shared<ExitButton>(dManager, UIFolderPath + "UI_TitleMasterSheet.dds", DirectX::SimpleMath::Vector2(674, 784), DirectX::SimpleMath::Vector2(1, 1), DirectX::SimpleMath::Vector2(572, 146), RECT{ (0), (292), (572), (438) }));
}

void ResourceManager::LoadGame(DeviceManager* dManager)
{
	std::shared_ptr<TitleImage> loadingImage = std::make_shared<TitleImage>(dManager, UIFolderPath + "UI_LoadingScreen.dds", DirectX::SimpleMath::Vector2(0, 0), DirectX::SimpleMath::Vector2(1, 1));

	// First render a loading screen to present to user while level data is being allocated/loaded
	RenderLoadScreen(dManager, loadingImage);

	LoadLevelsFromFile(dManager);

	GetCurrentMap()->GetCurrentZone()->RespawnPlayer();

	currentGameState = EGameStates::GAME;
}

// Goes through levels json file to add all needed level names to vector
void ResourceManager::LoadLevelsFromFile(DeviceManager* dManager)
{
	FILE* gLevelsFile;
	errno_t levelsStatus = fopen_s(&gLevelsFile, (GameFolderPath + "GameLevels.json").c_str(), "rb");
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

	rapidjson::Value levelsArray = levelsDoc["levels"].GetArray();
	for (int i = 0; i < levelsArray.Size(); i++)
	{
		std::string file = levelsArray[i].GetString();
		m_Levels.emplace_back(std::make_unique<Map>(this, dManager, (GameFolderPath + file).c_str()));
	}
}

void ResourceManager::SetCurrentMap(int _currentMapNum)
{
	currentMapNum = _currentMapNum;
}

void ResourceManager::LoadNextMap(DeviceManager* dManager)
{
	if (currentMapNum < m_Levels.size() - 1)
	{
		currentMapNum++;
		GetCurrentMap()->SetCurrentZoneNum(0);
		GetCurrentMap()->GetCurrentZone()->RespawnPlayer();
	}
	else
	{
		exit(EXIT_SUCCESS);
	}
}

void ResourceManager::LoadPreviousMap(DeviceManager* dManager)
{
	if (currentMapNum > 0)
	{
		currentMapNum--;
		GetCurrentMap()->SetCurrentZoneNum(0);
		GetCurrentMap()->GetCurrentZone()->RespawnPlayer();
	}
}

// Function to eventually load in .json files which will be used to create gameobjects and tiles for the map
void ResourceManager::ReloadMap(DeviceManager* dManager, int mapNum)
{
	GetCurrentMap()->GetCurrentZone()->RespawnPlayer();
}

Map::Map(ResourceManager* rManager, DeviceManager* dManager, const char* filePath)
{
	currentZoneNum = 0;

	FILE* fp;
	errno_t mapStatus = fopen_s(&fp, filePath, "rb");		// opens json file 
	char readBuffer[4096];
	rapidjson::FileReadStream mapStream(fp, readBuffer, sizeof(readBuffer));
	rapidjson::Document mapDocument;
	mapDocument.ParseStream(mapStream);		// parses json file 

	fclose(fp);		// closes json once it has been read 

	// Stores all the data from the json file in the respective variables from the class
	height = mapDocument["height"].GetInt();
	infinite = mapDocument["infinite"].GetBool();

	nextobjectid = mapDocument["nextobjectid"].GetInt();
	orientation = mapDocument["orientation"].GetString();
	renderorder = mapDocument["renderorder"].GetString();
	tileheight = mapDocument["tileheight"].GetInt();
	tilewidth = mapDocument["tilewidth"].GetInt();
	type = mapDocument["type"].GetString();
	width = mapDocument["width"].GetInt();

	rapidjson::Value tilesets = mapDocument["tilesets"].GetArray();
	ts_firstgid = tilesets[0]["firstgid"].GetInt();
	ts_source = tilesets[0]["source"].GetString();

	rapidjson::Value layersA = mapDocument["layers"].GetArray();
	for (size_t i = 0; i < layersA.Capacity(); i++)
	{
		layers.push_back(std::make_shared<Layer>(rManager, dManager, this, layersA[i]));
	}
}

void ResourceManager::LoadNextZone(DeviceManager* dManager)
{
	int zoneOffset = 1;
	if (GetCurrentMap()->GetCurrentZoneNum() < GetCurrentMap()->GetLayers().size() - zoneOffset)							// Check to see if incrementing zone num will go over max zones or not
	{
		GetCurrentMap()->SetCurrentZoneNum(GetCurrentMap()->GetCurrentZoneNum() + zoneOffset);
		GetCurrentMap()->GetCurrentZone()->RespawnPlayer();
	}
	else
	{
		LoadNextMap(dManager);
	}
}

void ResourceManager::LoadPreviousZone(DeviceManager* dManager)
{
	int zoneOffset = 1;
	if (GetCurrentMap()->GetCurrentZoneNum() > 0)
	{
		GetCurrentMap()->SetCurrentZoneNum(GetCurrentMap()->GetCurrentZoneNum() - zoneOffset);
		GetCurrentMap()->GetCurrentZone()->RespawnPlayer();
	}
	else
	{
		LoadPreviousMap(dManager);
	}
}

void ResourceManager::SavePlayerData()
{
	playerDataFile.open((PlayerFolderPath + "Player_SaveData.txt").c_str());										// Opens playerData text file 
	playerDataFile << GetCurrentMapNum();														// Writes players current map number to file
	playerDataFile << "\n";
	playerDataFile << GetCurrentMap()->GetCurrentZoneNum();										// Writes players current zone number to file
	playerDataFile.close();																		// Closes playerData text file
}

void ResourceManager::LoadPlayerData()
{
	playerDataFile.open((PlayerFolderPath + "Player_SaveData.txt").c_str());										// Opens playerData text file 

	std::string line;																			// String to temporarily store data being pulled from text file

	std::getline(playerDataFile, line);															// Grabs first number from text file which relates to the map number
	SetCurrentMap(stoi(line));																	// Converts map number from string to int and sets it 

	std::getline(playerDataFile, line);															// Grabs second number from text file which relates to the zone number
	GetCurrentMap()->SetCurrentZoneNum(stoi(line));												// Converts zone number from string to int and sets it 

	playerDataFile.close();																		// Closes playerData text file
}

void ResourceManager::RenderLoadScreen(DeviceManager* dManager, std::shared_ptr<TitleImage> loadingImage)
{
	dManager->BeginRender();

	dManager->GetSpriteBatch()->Begin(dManager->GetCommandList());

	loadingImage.get()->Render(dManager);

	dManager->GetSpriteBatch()->End();

	dManager->EndRender();
}

void ResourceManager::CheckTitleOnClickEvents(DeviceManager* dManager)
{
	DirectX::Mouse::State mouse = dManager->GetMouse()->GetState();

	if (GetAsyncKeyState(VK_LBUTTON))
	{
		POINT mousePos;
		GetCursorPos(&mousePos);
		for (auto& element : m_TitleUIElements)
		{
			if (mousePos.x > element.get()->GetCollisionBounds().left &&
				mousePos.x < element.get()->GetCollisionBounds().right &&
				mousePos.y > element.get()->GetCollisionBounds().top &&
				mousePos.y < element.get()->GetCollisionBounds().bottom)
			{
				element.get()->OnClicked(this, dManager);
			}
		}
	}
}

Layer::Layer(ResourceManager* rManager, DeviceManager* dManager, Map* ownerMap, rapidjson::Value& value)
{
	if (value.HasMember("data")) {
		for (auto& a : value["data"].GetArray()) 
		{
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

	LoadZoneTiles(rManager, dManager, ownerMap);
}

void Layer::RespawnPlayer()
{
	playerChar.get()->SetPosition(DirectX::SimpleMath::Vector2(playerSpawnPosition.x, playerSpawnPosition.y));
}

void Layer::LoadZoneTiles(ResourceManager* rManager, DeviceManager* dManager, Map* ownerMap)
{
	FILE* fp;
	errno_t tileSetStatus = fopen_s(&fp, (rManager->GameFolderPath + "Tileset.json").c_str(), "rb");
	char readBuffer[4096];
	rapidjson::FileReadStream mapStream(fp, readBuffer, sizeof(readBuffer));
	rapidjson::Document tilesetDoc;
	tilesetDoc.ParseStream(mapStream);
	fclose(fp);

	DirectX::SimpleMath::Vector2 objScale = { 6,6 };
	int collisionWidth = 0;
	int collisionHeight = 0;
	std::string objType;
	int collisionDirection;

	RECT playerRect = { 5,2,13,17 };
	RECT shotgunRect = { 5,2,13,17 };
	playerChar =  std::make_unique<Player>(dManager, rManager->PlayerFolderPath + "Player.dds", DirectX::SimpleMath::Vector2(0, 0), objScale, true, DirectX::SimpleMath::Vector2(8, 15), "Player", 0, playerRect);
	shotgunChar = std::make_unique<Shotgun>(dManager, rManager->ShotgunFolderPath + "Shotgun.dds", DirectX::SimpleMath::Vector2(0, 0), objScale, true, DirectX::SimpleMath::Vector2(9, 17), "Shotgun", 0, shotgunRect);

	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i] != 0)
		{
			size_t columns = tilesetDoc["columns"].GetInt();
			size_t val = data[i] - ownerMap->getFirstgid();										// Value of tile in tileset starting from 0 -> 63

			size_t x = val % columns;															// Position of tile on the tile map, (0,0) is top left going down and to the right
			size_t y = floor(val / columns);													// Floor rounds down (returns biggest int thats lower than original value)

			size_t xPos = i % ownerMap->getWidth();												//
			size_t yPos = floor(i / ownerMap->getWidth());										//

			float tileXPos = xPos * ownerMap->getTileWidth() * objScale.x;						// Tile object x and y position on screen
			float tileYPos = yPos * ownerMap->getTileWidth() * objScale.y;

			float x1 = x * ownerMap->getTileWidth();											// Pixel coordinates on tileset image, each corner of a tile square (like int rect from sfml)
			float x2 = (x + 1) * ownerMap->getTileWidth();
			float y1 = y * ownerMap->getTileHeight();
			float y2 = (y + 1) * ownerMap->getTileHeight();

			RECT tileRect;																		// Creating new RECT to pass to tile object constructor
			tileRect.left = x1;
			tileRect.right = x2;
			tileRect.top = y1;
			tileRect.bottom = y2;

			collisionWidth = tilesetDoc["tiles"].GetArray()[val]["objectgroup"].GetObj()["objects"].GetArray()[0]["width"].GetInt();			// Get collision bounds width and height from tileset json
			collisionHeight = tilesetDoc["tiles"].GetArray()[val]["objectgroup"].GetObj()["objects"].GetArray()[0]["height"].GetInt();
			objType = tilesetDoc["tiles"].GetArray()[val]["class"].GetString();
			collisionDirection = tilesetDoc["tiles"].GetArray()[val]["properties"].GetArray()[0]["value"].GetInt();

			if (objType == "Spawner")
			{
				playerSpawnPosition = DirectX::SimpleMath::Vector2(tileXPos, tileYPos - (playerChar->GetObjectSize().y * playerChar->GetScale().y));
			}

			tileObjects.push_back(std::make_shared<Tile>(dManager, rManager->GameFolderPath + "MasterSheet.dds", DirectX::SimpleMath::Vector2(tileXPos, tileYPos), objScale, true, DirectX::SimpleMath::Vector2(ownerMap->getTileWidth(), ownerMap->getTileHeight()), objType, collisionDirection, tileRect));
		}
	}
}