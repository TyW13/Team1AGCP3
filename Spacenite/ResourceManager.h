#pragma once
#include <vector>
#include <map>
#include <string>
#include "Texture.h"
#include "newGameObject.h"
#include "Respawner.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;

class Texture;
class GameObject;
class PlayerCharacter;
class Tile;

class Layer
{
public:
	Layer() {}
	Layer(Value& value);
	~Layer(){}

	std::vector<int> GetData() { return data; }
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

private:
	std::vector<int> data;
	std::string name;
	int id;
	int opacity;
	int height;
	int width;
	std::string image;
	std::string layersType;
	bool visible;
	int x;
	int y;
};

class Map
{
public:

	Map::Map(const char* filePath);

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

	std::vector<Layer> GetLayers() { return layers; }								// Getter function for layers vector (current system stores each map zone data)
	Layer GetCurrentZone() { return layers[currentZoneNum]; }						// Gets current layer (zone) to access its data)
	int GetCurrentZoneNum() { return currentZoneNum; }								// Gets current layer (zone) num
	void SetCurrentZoneNum(int zoneNum) { currentZoneNum = zoneNum; }				// Setter for currentZoneNum

private:
	int currentZoneNum;
	std::vector<Layer> layers;														// Layers are being used to represent each zone of a map

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
	ResourceManager(MyD3D& d3d){}

	void Init(MyD3D& d3d);
	void Update(MyD3D& d3d, float dTime);
	void Render(DirectX::DX11::SpriteBatch& batch);
	void Terminate();
	void LoadTexturesFromFile();													// Reads json file to obtain texture image file names from array (strings)
	void LoadLevelsFromFile();														// Reads json file to obtain level file names from array (strings)
	void CreateTexture(ID3D11Device& pDevice, const std::string &fPath);			// Creates a pointer to a texture object from texture image name string (Texture stores file name and path)
	void AddGameObject(MyD3D& d3d);													// (NOT CURRENTLYT IN USE) Function to add a gameobject to m_gObjects vector
	Texture* GetTexture(const std::string& tName);									// Getter for pointer to texture object in m_Textures vector
	std::string SetTexName(std::string path);										// Used to remove unneeded parts of texture path to get the images file name

	int GetCurrentMapNum() { return currentMapNum; }
	Map* GetCurrentMap()															// Getter for map object using currentMapNum set elsewhere
	{ 
		return m_Levels[currentMapNum]; 
	}
	void SetCurrentMap(int _currentMapNum);											// Set currentMapNum to given integer
	void LoadCurrentMap(MyD3D& d3d);												// Just reloads curren tmap
	void LoadNextMap(MyD3D& d3d);													// Increments currentMapNum by 1 and then uses new currentMapNum to call ReloadMap function
	void LoadPreviousMap(MyD3D& d3d);												// Decrements currentMapNum by 1 and then uses new currentMapNum to call ReloadMap function
	void ReloadMap(MyD3D& d3d, int mapNum);											// Load specific map by providing map num in vector

	void UnloadZone();																// Deletes all tile objects in vector to make room for the new zones tiles
	void LoadZoneInfo(MyD3D& d3d, int zoneNum);										// Load specific map by providing map num in vector
	void LoadNextZone(MyD3D& d3d);													// Increments currentMapNum by 1 and then uses new currentMapNum to call ReloadMap function
	void LoadPreviousZone(MyD3D& d3d);												// Decrements currentMapNum by 1 and then uses new currentMapNum to call ReloadMap function

	GameObject* GetGameObject(int objNum) { return m_gObjects[objNum]; }
	std::vector<GameObject*> GetObjects() { return m_gObjects; }
	std::vector<Tile*> GetTiles() { return m_Tiles; }
	Respawner* GetCurrentSpawner() { return currentSpawner; }

private:
	std::vector<Map*> m_Levels;														// Vector to store pointers to Map objects
	std::map<std::string, Texture*> m_Textures;										// Map to store Texture object and its respective texture name string
	std::vector<GameObject*> m_gObjects;											// Vector which will store collidable objects, reducing the collisions needing to check
	std::vector<Tile*> m_Tiles;														// Vector to store current zone tiles
	int currentMapNum;					
	Respawner* currentSpawner;

	int collisionWidth;
	int collisionHeight;
};