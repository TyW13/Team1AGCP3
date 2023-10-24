#pragma once
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

#include <fstream>;

#include "Player.h"
#include "Shotgun.h"

class Map;

class Layer
{
public:
	Layer() {}
	Layer(DeviceManager* dManager, Map* ownerMap, rapidjson::Value& value);
	~Layer(){}

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

	void RespwanPlayer();
	void LoadZoneTiles(DeviceManager* dManager, Map* ownerMap);

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

	Map::Map(DeviceManager* dManager, const char* filePath);

	~Map() {}

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
	int GetColumns() { return ts_Columns; }

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

	// Tile set vars (since we are using a single tileset per level)
	int ts_Firstgid;
	int ts_Columns;
	std::string ts_Image;
	int ts_ImageHeight;
	int ts_ImageWidth;
	int ts_Margin;
	std::string ts_Name;
	int ts_Spacing;
	int ts_Tilecount;
	int ts_Tileheight;
	int ts_Tilewidth;
	std::string ts_Type;

	int ts_firstgid;																//refers to which tile set was used to create the map 
	std::string ts_source;															//links to the tile set used to create map 
};

class ResourceManager
{
public:
	ResourceManager(){}
	~ResourceManager()
	{
		Terminate();
	}

	void Init(DeviceManager* dManager);
	void Update(DeviceManager* dManager, float dTime);
	void Render(DeviceManager* dManager);
	void Terminate();

	void LoadLevelsFromFile(DeviceManager* dManager);														// Reads json file to obtain level file names from array (strings)

	int GetCurrentMapNum() { return currentMapNum; }
	Map* GetCurrentMap()															// Getter for map object using currentMapNum set elsewhere
	{ 
		return m_Levels[currentMapNum].get(); 
	}
	void SetCurrentMap(int _currentMapNum);											// Set currentMapNum to given integer
	void LoadCurrentMap(DeviceManager* dManager);												// Just reloads current map
	void LoadNextMap(DeviceManager* dManager);													// Increments currentMapNum by 1 and then uses new currentMapNum to call ReloadMap function
	void LoadPreviousMap(DeviceManager* dManager);												// Decrements currentMapNum by 1 and then uses new currentMapNum to call ReloadMap function
	void ReloadMap(DeviceManager* dManager, int mapNum);											// Load specific map by providing map num in vector

	void UnloadZone();																// Deletes all tile objects in vector to make room for the new zones tiles
	void LoadZoneInfo(DeviceManager* dManager, int zoneNum);										// Load specific map by providing map num in vector
	void LoadNextZone(DeviceManager* dManager);													// Increments currentMapNum by 1 and then uses new currentMapNum to call ReloadMap function
	void LoadPreviousZone(DeviceManager* dManager);												// Decrements currentMapNum by 1 and then uses new currentMapNum to call ReloadMap function

	std::vector<GameObject*> GetObjects() { return m_Objects; }
	//Player* GetPlayer() { return playerChar; }

	void SavePlayerData();															// Attach to button in the UI allowing player to save their current Map and Zone data to text file 
	void LoadPlayerData();															// Reads Map and Zone data from text file and sets player to that specific level upon loading game 

private:
	std::vector<std::shared_ptr<GameObject>> tileSet;


	std::vector<std::unique_ptr<Map>> m_Levels;										// Vector to store pointers to Map objects
	//std::vector<std::wstring> m_TexPaths;
	//Player* playerChar = NULL;														// NOTE: HAS BEEN MOVED
	//Shotgun* shotgunChar = NULL;													// NOTE: HAS BEEN MOVED
	std::vector<GameObject*> m_Objects;												// Vector to store current zone tiles
	int currentMapNum = 0;					

	std::fstream playerDataFile;													// Stores Map and Zone num to enable them to save/load progress
};