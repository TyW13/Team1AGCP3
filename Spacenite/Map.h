#pragma once

#include "rapidjson/document.h"
#include <vector>
#include <string>


class Map								// This class represents all the data from the json after it has been parsed
{
public:

	Map(const char* map);
	~Map();

	int getHeight() { return height; }
	bool isInfinite() { return infinite; }
	std::vector<Layers> getLayers() { return layers; }
	int getNextLayerId() { return nextlayerid; }
	int getNextObjectId() { return nextobjectid; }
	std::string getOrientation() { return orientation; }
	std::string getRenderOrder() { return renderorder; }
	std::string getTiledVersion() { return tiledversion; }
	int getTileHeight() { return tileheight; }
	std::vector<TileSetMap> getTileSetMap() { return tilesets; }
	int getTileWidth() { return tilewidth; }
	std::string getType() { return type; }
	float getVersion() { return version; }
	int getWidth() { return width; }

private:

	int height;
	bool infinite;
	std::vector<Layers> layers;
	int nextlayerid;
	int nextobjectid;
	std::string orientation;
	std::string renderorder;
	std::string tiledversion;
	int tileheight;
	std::vector<TileSetMap> tilesets;
	int tilewidth;
	std::string type;
	float version;
	int width;

	rapidjson::Document document;

};

class Layers
{
public:

	Layers(rapidjson::Value& value);
	~Layers();

	std::vector<int> getData() { return data; }
	int getHeight() { return height; }
	int getId() { return id; }
	std::string getImage() { return image; }
	std::string getName() { return name; }
	int getOpacity() { return opacity; }
	std::string getType() { return type; }
	bool isVisible() { return visible; }
	int getWidth() { return width; }
	int getX() { return x; }
	int getY() { return y; }

private:

	std::vector<int> data;
	int height;
	int id;
	std::string image;
	std::string name;
	int opacity;
	std::string type;
	bool visible;
	int width;
	int x;
	int y;
};

class TileSetMap
{
public:

	TileSetMap(rapidjson::Value& value) {
		firstgid = value["firstgid"].GetInt();
		source = value["source"].GetString();
	}

	~TileSetMap() {}

	int getFirstGId() { return firstgid; }
	std::string getSource() { return source; }

private: 

	int firstgid;
	std::string source;
};

