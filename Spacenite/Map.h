#pragma once

#include "rapidjson/document.h"
#include <vector>
#include <string>

//class TileSetMap
//{
//public:
//
//	TileSetMap(rapidjson::Value& value);
//
//	~TileSetMap() {}
//
//	int getFirstGId() { return firstgid; }
//	std::string getSource() { return source; }
//
//private:
//
//	int firstgid;
//	std::string source;
//};

//class Layers
//{
//public:
//
//	Layers(rapidjson::Value& value);
//
//	~Layers() {}
//
//	std::vector<int> getData() { return data; }
//	int getHeight() { return height; }
//	int getId() { return id; }
//	std::string getImage() { return image; }
//	std::string getName() { return name; }
//	int getOpacity() { return opacity; }
//	std::string getType() { return type; }
//	bool isVisible() { return visible; }
//	int getWidth() { return width; }
//	int getX() { return x; }
//	int getY() { return y; }
//
//private:
//
//	std::vector<int> data;
//	int height;
//	int id;
//	std::string image;
//	std::string name;
//	int opacity;
//	std::string type;
//	bool visible;
//	int width;
//	int x;
//	int y;
//};

class Map					// This class represents all the data from the json after it has been parsed
{
public:

	Map::Map();

	~Map() {}

	// Layers array functions 
	int getID() { return id; }
	std::string getName() { return name; }
	int getOpacity() { return opacity; }
	std::string getLayersType() { return layersType; }
	bool getVisible() { return visible; }
	int getX() { return x; }
	int getY() { return y; }

	int getHeight() { return height; }
	bool isInfinite() { return infinite; }
	int getNextLayerId() { return nextlayerid; }
	int getNextObjectId() { return nextobjectid; }
	std::string getOrientation() { return orientation; }
	std::string getRenderOrder() { return renderorder; }
	std::string getTiledVersion() { return tiledversion; }
	int getTileHeight() { return tileheight; }
	int getTileWidth() { return tilewidth; }
	std::string getType() { return type; }
	int getWidth() { return width; }

	// Tilesets array functions 
	int getFirstgid() { return firstgid; }
	std::string getSource() { return source; }

private:

	// Layers array data
	int id;
	std::string name;
	int opacity;
	std::string layersType;
	bool visible;
	int x;
	int y;

	int height;
	bool infinite;
	int nextlayerid;
	int nextobjectid;
	std::string orientation;
	std::string renderorder;
	std::string tiledversion;
	int tileheight;
	int tilewidth;
	std::string type;
	int width;

	// Tilesets array data
	int firstgid;
	std::string source;
};



