//#pragma once
//#include "pch.h"
//
//#include "rapidjson/document.h"
//#include "rapidjson/filereadstream.h"
//
//using namespace rapidjson;
//
//class Texture;
//class GameObject;
//class PlayerCharacter;
//class Tile;
//
//class Layer
//{
//public:
//	Layer() {}
//	Layer(Value& value);
//	~Layer(){}
//
//	std::vector<int> GetData() { return data; }
//	std::string GetName() { return name; }
//	int GetId() { return id; }
//	int GetOpacity() { return opacity; }
//	int GetHeight() { return height; }
//	int GetWidth() { return width; }
//	std::string GetImage() { return image; }
//	std::string GetLayersType() { return layersType; }
//	bool IsVisible() { return visible; }
//	int GetX() { return x; }
//	int GetY() { return y; }
//
//private:
//	std::vector<int> data;
//	std::string name;
//	int id;
//	int opacity;
//	int height;
//	int width;
//	std::string image;
//	std::string layersType;
//	bool visible;
//	int x;
//	int y;
//};
//
//class Map
//{
//public:
//
//	Map::Map(const char* filePath);
//
//	~Map() {}
//
//	int getHeight() { return height; }
//	bool isInfinite() { return infinite; }
//	int getNextObjectId() { return nextobjectid; }
//	std::string getOrientation() { return orientation; }
//	std::string getRenderOrder() { return renderorder; }
//	int getTileHeight() { return tileheight; }
//	int getTileWidth() { return tilewidth; }
//	std::string getType() { return type; }
//	int getWidth() { return width; }
//
//	// Tilesets array functions 
//	int getFirstgid() { return ts_firstgid; }
//	std::string getSource() { return ts_source; }
//	int GetColumns() { return ts_Columns; }
//
//	std::vector<Layer> GetLayers() { return layers; }								// Getter function for layers vector (current system stores each map zone data)
//	Layer GetCurrentZone() { return layers[currentZoneNum]; }						// Gets current layer (zone) to access its data)
//	int GetCurrentZoneNum() { return currentZoneNum; }								// Gets current layer (zone) num
//	void SetCurrentZoneNum(int zoneNum) { currentZoneNum = zoneNum; }				// Setter for currentZoneNum
//
//private:
//	int currentZoneNum;
//	std::vector<Layer> layers;														// Layers are being used to represent each zone of a map
//
//	int height;																		// height of the map in tiles 
//	bool infinite;																	// refers to if map has infinite height/width 
//	int nextobjectid;
//	std::string orientation;														// shape of each tile - "orthogonal" = square tiles 
//	std::string renderorder;														// the order that tiles are rendered in - "right-down" = begins rendering from the top left corner of map 
//	int tileheight;																	// height of one tile 
//	int tilewidth;																	// width of one tile 
//	std::string type;
//	int width;																		// width of the map in tiles 
//
//	// Tilesets data
//
//	// Tile set vars (since we are using a single tileset per level)
//	int ts_Firstgid;
//	int ts_Columns;
//	std::string ts_Image;
//	int ts_ImageHeight;
//	int ts_ImageWidth;
//	int ts_Margin;
//	std::string ts_Name;
//	int ts_Spacing;
//	int ts_Tilecount;
//	int ts_Tileheight;
//	int ts_Tilewidth;
//	std::string ts_Type;
//
//	int ts_firstgid;																//refers to which tile set was used to create the map 
//	std::string ts_source;															//links to the tile set used to create map 
//};
//
//class ResourceManager
//{
//public:
//	ResourceManager(){}
//
//	void Init();
//	void Update(float dTime);
//	void Render(DirectX::SpriteBatch& batch);
//	void Terminate();
//	void LoadTexturesFromFile();													// Reads json file to obtain texture image file names from array (strings)
//	void LoadLevelsFromFile();														// Reads json file to obtain level file names from array (strings)
//	//void CreateTexture(ID3D11Device& pDevice, const std::string &fPath);			// Creates a pointer to a texture object from texture image name string (Texture stores file name and path)
//	Texture* GetTexture(const std::string& tName);									// Getter for pointer to texture object in m_Textures vector
//	std::string SetTexName(std::string path);										// Used to remove unneeded parts of texture path to get the images file name
//
//	int GetCurrentMapNum() { return currentMapNum; }
//	Map* GetCurrentMap()															// Getter for map object using currentMapNum set elsewhere
//	{ 
//		return m_Levels[currentMapNum]; 
//	}
//	void SetCurrentMap(int _currentMapNum);											// Set currentMapNum to given integer
//	void ReloadMap(int mapNum);											// Load specific map by providing map num in vector
//
//	void UnloadZone();																// Deletes all tile objects in vector to make room for the new zones tiles
//	void LoadZoneInfo(int zoneNum);										// Load specific map by providing map num in vector
//
//private:
//	std::vector<Map*> m_Levels;														// Vector to store pointers to Map objects
//	std::vector<Tile*> m_Tiles;														// Vector to store current zone tiles
//	int currentMapNum;					
//
//	int collisionWidth;
//	int collisionHeight;
//};