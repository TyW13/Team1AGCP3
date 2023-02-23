#pragma once
#include <vector>
#include <map>
#include <string>
#include "Texture.h"
#include "newGameObject.h"
#include "PlayerCharacter.h"
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
	~Layer() {}

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

	// All functions are below are Getters that return the value requested 

	// Layers array functions 
	//int getID() { return id; }
	//std::string getName() { return name; }
	//int getOpacity() { return opacity; }
	//std::string getLayersType() { return layersType; }
	//bool getVisible() { return visible; }
	//int getX() { return x; }
	//int getY() { return y; }

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
	std::vector<Tile*> GetTiles() { return tiles; }
	std::vector<RECTF> GetTilesRects() { return tileRects; }
	std::vector<Vector2> GetTilePositions() { return tilePositions; }


	std::vector<Layer> GetLayers() { return layers; }
	Layer GetCurrentZone() { return layers[currentZoneNum]; }
	int GetCurrentZoneNum() { return currentZoneNum; }
	void SetCurrentZoneNum(int zoneNum) { currentZoneNum = zoneNum; }

private:
	int currentZoneNum;
	std::vector<Layer> layers;				// Layers are being used to represent each zone of a map

	int height;							// height of the map in tiles 
	bool infinite;						// refers to if map has infinite height/width 
	int nextobjectid;
	std::string orientation;			// shape of each tile - "orthogonal" = square tiles 
	std::string renderorder;			// the order that tiles are rendered in - "right-down" = begins rendering from the top left corner of map 
	int tileheight;						// height of one tile 
	int tilewidth;						// width of one tile 
	std::string type;
	int width;							// width of the map in tiles 

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

	int ts_firstgid;						 //refers to which tile set was used to create the map 
	std::string ts_source;					 //links to the tile set used to create map 
	std::vector<Tile*> tiles;
	std::vector<RECTF> tileRects;
	std::vector<Vector2> tilePositions;
};

class ResourceManager
{
	// Commented out copy and move constructors and assignment operators

	//ResourceManager(const ResourceManager& other)												// Copy constructor
	//{
	//	this->m_Textures = other.m_Textures;
	//	this->m_gObjects = other.m_gObjects;
	//	this->pTex = other.pTex;
	//	this->pObj = other.pObj;
	//}
	//
	//ResourceManager& operator=(const ResourceManager& rhs)
	//{
	//	if (this != &rhs)
	//	{
	//		this->m_Textures = rhs.m_Textures;
	//		this->m_gObjects = rhs.m_gObjects;
	//
	//		if (this->pTex)
	//		{
	//			delete[] this->pTex;
	//		}
	//		if (this->pObj)
	//		{
	//			delete[] this->pObj;
	//		}
	//
	//		this->pTex = rhs.pTex;
	//		this->pObj = rhs.pObj;
	//	}
	//}
	//
	//ResourceManager(ResourceManager&& other)
	//{
	//	this->m_Textures = other.m_Textures;
	//	this->m_gObjects = other.m_gObjects;
	//	this->pTex = other.pTex;
	//	this->pObj = other.pObj;
	//
	//	other.pTex = nullptr;
	//	other.pObj = nullptr;
	//}
	//
	//ResourceManager& operator=(ResourceManager&& rhs)											// Move assignment
	//{
	//	// Self assignment check
	//	if (this != &rhs)
	//	{
	//		this->m_Textures = rhs.m_Textures;
	//		this->m_gObjects = rhs.m_gObjects;
	//
	//		if (this->pTex)
	//		{
	//			delete this->pTex;
	//		}
	//		if (this->pObj)
	//		{
	//			delete this->pObj;
	//		}
	//
	//		// Deleting the old data
	//		this->pObj = rhs.pObj;
	//		rhs.pObj = nullptr;
	//		this->pTex = rhs.pTex;
	//		rhs.pTex = nullptr;
	//	}
	//
	//	return *this;
	//}

public:
	ResourceManager(MyD3D& d3d){}

	void Init(ID3D11Device& pDevice, MyD3D& d3d);
	void Update(MyD3D& d3d, float dTime);
	void Render(DirectX::DX11::SpriteBatch& batch);
	void Terminate();
	void LoadTexturesFromFile();
	void LoadLevelsFromFile();
	void CreateTexture(ID3D11Device& pDevice, const std::string &fPath);
	void AddGameObject(MyD3D& d3d);
	Texture* GetTexture(const std::string& tName);
	std::string SetTexName(std::string path);

	Map* GetCurrentMap() 
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

private:
	std::vector<Map*> m_Levels;														// Vector to store pointers to Map objects
	std::map<std::string, Texture*> m_Textures;										// Map to store Texture object and its respective texture name string
	std::vector<GameObject*> m_gObjects;											// Vector which will store collidable objects, reducing the collisions needing to check
	std::vector<Tile*> m_Tiles;														// Vector to store current zone tiles
	int currentMapNum;																// 
};