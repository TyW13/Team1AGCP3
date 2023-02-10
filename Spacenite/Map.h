#pragma once

#include "rapidjson/document.h"
#include <vector>
#include <string>
#include <map>

// TEST CLASS
class LayerData 
{
public:
	std::vector<int> GetData() { return data; };
	int GetHeight() { return height; };
	int GetId() { return id; };
	std::string GetName() { return name; };
	float GetOpacity() { return opacity; };
	std::string GetType() { return type; };
	bool IsVisible() { return visible; };
	float GetWidth() { return width; };
	float GetX() { return x; };
	float GetY() { return y; };

private:
	std::vector<int> data;
	int height;
	int id;
	std::string name;
	float opacity;
	std::string type;
	bool visible;
	float width;
	float x;
	float y;
};

class TileSet
{
public:

	TileSet(){}
	TileSet(int firstgid, const char* tileset);
	~TileSet();

	int getFirstGid() { return firstgid; }
	int getColumns() { return columns; }
	std::string getImaage() { return image; }
	int getImageHeight() { return imageHeight; }
	int getImageWidth() { return imageWidth; }
	int getMargin() { return margin; }
	std::string getName() { return name; }
	int getSpacing(){ return spacing; }
	int getTileCOunt() { return tilecount; }
	int getTileHeight() { return tileheight; }
	int getTIleWidth() { return tilewidth; }
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
	std::string tiledversion;
	int tileheight;
	int tilewidth;
	std::string type;
	float version;
};

class TileSetMap
{
public:

	TileSetMap(rapidjson::Value& value);

	~TileSetMap() {}

	int getFirstGId() { return firstgid; }
	std::string getSource() { return source; }

private:

	int firstgid;
	std::string source;
};

class Layers
{
public:

	Layers(rapidjson::Value& value);

	~Layers() {}

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

class Map								// This class represents all the data from the json after it has been parsed
{
public:

	Map::Map();

	~Map() {}

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
	int getWidth() { return width; }

private:

	int height;
	bool infinite;
	std::vector<Layers> layers;
	std::map<std::string, LayerData> layerData;
	int nextlayerid;
	int nextobjectid;
	std::string orientation;
	std::string renderorder;
	std::string tiledversion;
	int tileheight;
	std::vector<TileSetMap> tilesets;
	int tilewidth;
	std::string type;
	int width;
};



