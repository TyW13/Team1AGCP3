#pragma once
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

#include <fstream>;

#include "TitleImage.h"
#include "Player.h"
#include "Shotgun.h"

class Map;
class UIElement;

class Layer
{
public:
	Layer() {}
	Layer(ResourceManager* rManager, DeviceManager* dManager, Map* ownerMap, rapidjson::Value& value);

	std::vector<int> GetData() { return data; }
	std::vector<std::shared_ptr<GameObject>> GetTiles() { return tileObjects; }
	std::string GetName() { return name; }
	int GetId() { return id; }
	int GetOpacity() { return opacity; }
	int GetHeight() { return height; }
	int GetWidth() { return width; }
	std::string GetImage() { return image; }
	std::string GetLayersType() { return layersType; }
	bool IsVisible() { return visible; }
	int GetX() { return x; }
	int GetY() { return y; }

	Player* GetPlayer() { return playerChar.get(); }
	Shotgun* GetShotgunObj() { return shotgunChar.get(); }

	void RespawnPlayer();
	void LoadZoneTiles(ResourceManager* rManager, DeviceManager* dManager, Map* ownerMap);

private:
	std::vector<int> data;
	std::vector<std::shared_ptr<GameObject>> tileObjects;
	std::string name = "";
	int id = 0;
	int opacity = 0;
	int height = 0;
	int width = 0;
	std::string image = "";
	std::string layersType = "";
	bool visible = false;
	int x = 0;
	int y = 0;
	DirectX::SimpleMath::Vector2 playerSpawnPosition = { 0, 0 };

	std::unique_ptr<Player> playerChar;
	std::unique_ptr<Shotgun> shotgunChar;
};

class Map
{
public:

	Map(){}
	Map::Map(ResourceManager* rManager, DeviceManager* dManager, const char* filePath);

	int getHeight() { return height; }
	bool isInfinite() { return infinite; }
	int getNextObjectId() { return nextobjectid; }
	std::string getOrientation() { return orientation; }
	std::string getRenderOrder() { return renderorder; }
	int getTileHeight() { return tileheight; }
	int getTileWidth() { return tilewidth; }
	std::string getType() { return type; }
	int getWidth() { return width; }

	// Tilesets array functions 
	int getFirstgid() { return ts_firstgid; }
	std::string getSource() { return ts_source; }

	std::vector<std::shared_ptr<Layer>> GetLayers() { return layers; }								// Getter function for layers vector (current system stores each map zone data)
	Layer* GetCurrentZone() { return layers[currentZoneNum].get(); }						// Gets current layer (zone) to access its data)
	int GetCurrentZoneNum() { return currentZoneNum; }								// Gets current layer (zone) num
	void SetCurrentZoneNum(int zoneNum) { currentZoneNum = zoneNum; }				// Setter for currentZoneNum

private:
	int currentZoneNum;
	std::vector<std::shared_ptr<Layer>> layers;														// Layers are being used to represent each zone of a map

	int height;																		// height of the map in tiles 
	bool infinite;																	// refers to if map has infinite height/width 
	int nextobjectid;
	std::string orientation;														// shape of each tile - "orthogonal" = square tiles 
	std::string renderorder;														// the order that tiles are rendered in - "right-down" = begins rendering from the top left corner of map 
	int tileheight;																	// height of one tile 
	int tilewidth;																	// width of one tile 
	std::string type;
	int width;																		// width of the map in tiles 

	// Tilesets data

	int ts_firstgid;																//refers to which tile set was used to create the map 
	std::string ts_source;															//links to the tile set used to create map 
};

class ResourceManager
{
public:
	ResourceManager(){}

	void Init(DeviceManager* dManager);
	void Update(DeviceManager* dManager, float dTime);
	void Render(DeviceManager* dManager);

	void AddTitleScreenUI(DeviceManager* dManager);
	void LoadGame(DeviceManager* dManager);
	void LoadLevelsFromFile(DeviceManager* dManager);														// Reads json file to obtain level file names from array (strings)

	int GetCurrentMapNum() { return currentMapNum; }
	Map* GetCurrentMap()															// Getter for map object using currentMapNum set elsewhere
	{ 
		return m_Levels[currentMapNum].get(); 
	}
	void SetCurrentMap(int _currentMapNum);											// Set currentMapNum to given integer
	void LoadNextMap(DeviceManager* dManager);													// Increments currentMapNum by 1 and then uses new currentMapNum to call ReloadMap function
	void LoadPreviousMap(DeviceManager* dManager);												// Decrements currentMapNum by 1 and then uses new currentMapNum to call ReloadMap function
	void ReloadMap(DeviceManager* dManager, int mapNum);											// Load specific map by providing map num in vector

	void LoadNextZone(DeviceManager* dManager);													// Increments currentMapNum by 1 and then uses new currentMapNum to call ReloadMap function
	void LoadPreviousZone(DeviceManager* dManager);												// Decrements currentMapNum by 1 and then uses new currentMapNum to call ReloadMap function


	void SavePlayerData();															// Attach to button in the UI allowing player to save their current Map and Zone data to text file 
	void LoadPlayerData();															// Reads Map and Zone data from text file and sets player to that specific level upon loading game 

	const std::string GameFolderPath = "Data/Game/";
	const std::string PlayerFolderPath = "Data/Player/";
	const std::string ShotgunFolderPath = "Data/Shotgun/";
	const std::string UIFolderPath = "Data/UI/";

private:
	void RenderLoadScreen(DeviceManager* dManager, std::shared_ptr<TitleImage> loadingImage);
	void CheckTitleOnClickEvents(DeviceManager* dManager);														// Checking On Click "events" for buttons on title screen

	std::vector<std::shared_ptr<Map>> m_Levels;										// Vector to store pointers to Map objects
	std::vector<std::shared_ptr<UIElement>> m_TitleUIElements;
	int currentMapNum = 0;					

	std::fstream playerDataFile;													// Stores Map and Zone num to enable them to save/load progress

	std::unique_ptr<TitleImage> titleImage;

	enum EGameStates
	{
		TITLE,
		GAME,
		CREDITS
	};
	EGameStates currentGameState = EGameStates::TITLE;
};