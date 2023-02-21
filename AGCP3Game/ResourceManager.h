#pragma once
#include <vector>
#include <map>
#include <string>
#include "d3dx12.h"

#include "SimpleMath.h"

#include "Texture.h"
#include "newGameObject.h"
#include "PlayerCharacter.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "DeviceResources.h"
#include <SpriteBatch.h>



using namespace rapidjson;

class Texture;
class GameObject;
class PlayerCharacter;
class Tile;

//class TileSet
//{
//public:
//	TileSet::TileSet() {}
//	//	TileSet::TileSet(int firstgid, const char* tileset);
//	~TileSet() {}
//
//	void Init()
//	{
//
//	}
//
//	void LoadTileSetData(int firstgid, const char* tileset);
//	int getFirstGid() { return firstgid; }
//	int getColumns() { return columns; }
//	std::string getImaage() { return image; }
//	int getImageHeight() { return imageHeight; }
//	int getImageWidth() { return imageWidth; }
//	int getMargin() { return margin; }
//	std::string getName() { return name; }
//	int getSpacing() { return spacing; }
//	int getTileCount() { return tilecount; }
//	int getTileHeight() { return tileheight; }
//	int getTileWidth() { return tilewidth; }
//	std::string getType() { return type; }
//private:
//	int firstgid;
//	int columns;
//	std::string image;
//	int imageHeight;
//	int imageWidth;
//	int margin;
//	std::string name;
//	int spacing;
//	int tilecount;
//	int tileheight;
//	int tilewidth;
//	std::string type;
//};

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

	//// Layers array data
	//int id;								// layer ID 
	//std::string name;					// name of layer 
	//int opacity;						// how transparent layer is between 0-1 
	//std::string layersType;				// type of layer 
	//bool visible;						// whether layer is shown or hidden 
	//int x;								// horizontal layer offset
	//int y;								// vertical layer offset

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
	ResourceManager(ID3D12Device& pDevice) {}

	
	void Init(ID3D12Device& pDevice);

	void Update(float dTime);
	void Render(DirectX::DX12::SpriteBatch& batch);
	void Terminate();
	void LoadLevelsFromFile();
	void CreateTexture(ID3D12Device& pDevice, const std::string& fPath);
	void AddGameObject(ID3D12Device& pDevice);
	Texture* GetTexture(const std::string& tName);
	std::string SetTexName(std::string path);

	Map GetCurrentMap() { return m_Levels[currentMapNum]; }
	void SetCurrentMap(int _currentMapNum);											            // Set currentMapNum to given integer
	void LoadCurrentMap(ID3D12Device& pDevice);
	void LoadNextMap(ID3D12Device& pDevice);													// Increments currentMapNum by 1 and then uses new currentMapNum to call ReloadMap function
	void LoadPreviousMap(ID3D12Device& pDevice);												// Decrements currentMapNum by 1 and then uses new currentMapNum to call ReloadMap function
	void ReloadMap(ID3D12Device& pDevice, int mapNum);											// Load specific map by providing map num in vector

	void UnloadZone();
	void LoadZoneInfo(ID3D12Device& pDevice, int zoneNum);										// Load specific map by providing map num in vector
	void LoadNextZone(ID3D12Device& pDevice);													// Increments currentMapNum by 1 and then uses new currentMapNum to call ReloadMap function
	void LoadPreviousZone(ID3D12Device& pDevice);												// Decrements currentMapNum by 1 and then uses new currentMapNum to call ReloadMap function
	void RenderTiles();

private:
	std::vector<Map> m_Levels;
	std::map<std::string, Texture*> m_Textures;
	std::vector<GameObject*> m_gObjects;
	std::vector<std::vector<Tile*>> m_Tiles;
	int currentMapNum;
};