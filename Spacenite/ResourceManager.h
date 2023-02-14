#pragma once
#include <vector>
#include <map>
#include <string>
#include "Texture.h"
#include "newGameObject.h"
#include "PlayerCharacter.h"

//using namespace DirectX;

class Texture;
class GameObject;
class PlayerCharacter;

class Map
{
public:

	Map::Map(const char* filePath);

	~Map() {}

	// All functions are below are Getters that return the value requested 

	// Layers array functions 
	int getID() { return id; }
	std::string getName() { return name; }
	std::vector<int> getData() { return data; }
	int getOpacity() { return opacity; }
	std::string getLayersType() { return layersType; }
	bool getVisible() { return visible; }
	int getX() { return x; }
	int getY() { return y; }

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
	int getFirstgid() { return firstgid; }
	std::string getSource() { return source; }

private:

	// Layers array data
	int id;								// layer ID 
	std::string name;					// name of layer 
	int opacity;						// how transparent layer is between 0-1 
	std::string layersType;				// type of layer 
	bool visible;						// whether layer is shown or hidden 
	int x;								// horizontal layer offset
	int y;								// vertical layer offset

	std::vector<int> data;				// Data from layers data array
	int height;							// height of the map in tiles 
	bool infinite;						// refers to if map has infinite height/width 
	int nextobjectid;
	std::string orientation;			// shape of each tile - "orthogonal" = square tiles 
	std::string renderorder;			// the order that tiles are rendered in - "right-down" = begins rendering from the top left corner of map 
	int tileheight;						// height of one tile 
	int tilewidth;						// width of one tile 
	std::string type;
	int width;							// width of the map in tiles 

	// Tilesets array data

	int firstgid;						// refers to which tile set was used to create the map 
	std::string source;					// links to the tile set used to create map 
};

class TileSet
{
public:
	TileSet::TileSet() {}
	//	TileSet::TileSet(int firstgid, const char* tileset);
	~TileSet() {}

	void Init()
	{

	}

	void LoadTileSetData(int firstgid, const char* tileset);
	int getFirstGid() { return firstgid; }
	int getColumns() { return columns; }
	std::string getImaage() { return image; }
	int getImageHeight() { return imageHeight; }
	int getImageWidth() { return imageWidth; }
	int getMargin() { return margin; }
	std::string getName() { return name; }
	int getSpacing() { return spacing; }
	int getTileCount() { return tilecount; }
	int getTileHeight() { return tileheight; }
	int getTileWidth() { return tilewidth; }
	std::string getType() { return type; }
private:
	int firstgid;
	int columns;
	std::string image;
	int imageHeight;
	int imageWidth;
	int margin;
	std::string name;
	int spacing;
	int tilecount;
	int tileheight;
	int tilewidth;
	std::string type;
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
	void Update(float dTime);
	void Render(DirectX::DX11::SpriteBatch& batch);
	void Terminate();
	void CreateTexture(ID3D11Device& pDevice, const std::string &fPath);
	void AddGameObject(MyD3D& d3d);
	Texture* GetTexture(const std::string& tName);
	std::string SetTexName(std::string path);
	void LoadJSON();
	void LoadTileSet(Map map);
private:
	std::map<std::string, Texture*> m_Textures;
	std::vector<GameObject*> m_gObjects;


	// Tile set vars
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

	std::vector<RECTF> tileRects;
	std::vector<Vector2> tilePositions;
};