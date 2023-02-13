#pragma once
<<<<<<< Updated upstream
#include <string>
=======

#include <vector>
#include <string>
#include <map>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;

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

	TileSet::TileSet();
	TileSet::TileSet(int firstgid, const char* tileset);
	~TileSet(){}

	int getFirstGid() { return firstgid; }
	int getColumns() { return columns; }
	std::string getImaage() { return image; }
	int getImageHeight() { return imageHeight; }
	int getImageWidth() { return imageWidth; }
	int getMargin() { return margin; }
	std::string getName() { return name; }
	int getSpacing(){ return spacing; }
	int getTileCount() { return tilecount; }
	int getTileHeight() { return tileheight; }
	int getTileWidth() { return tilewidth; }
	std::string getType() { return type; }
>>>>>>> Stashed changes

class Map
{
public:

<<<<<<< Updated upstream
	Map::Map();

	~Map() {}
=======
	TileSetMap(Value& value);

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
	Layers(){}

	Layers(Value& value);
>>>>>>> Stashed changes

	// All functions are below are Getters that return the value requested 

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
	int id;								// layer ID 
	std::string name;					// name of layer 
	int opacity;						// how transparent layer is between 0-1 
	std::string layersType;				// type of layer 
	bool visible;						// whether layer is shown or hidden 
	int x;								// horizontal layer offset
	int y;								// vertical layer offset

	int height;							// height of the map in tiles 
	bool infinite;						// refers to if map has infinite height/width 
	int nextlayerid;
	int nextobjectid;
	std::string orientation;			// shape of each tile - "orthogonal" = square tiles 
	std::string renderorder;			// the order that tiles are rendered in - "right-down" = begins rendering from the top left corner of map 
	std::string tiledversion;			// tiled version number 
	int tileheight;						// height of one tile 
	int tilewidth;						// width of one tile 
	std::string type;
	int width;							// width of the map in tiles 

	// Tilesets array data
	int firstgid;						// refers to which tile set was used to create the map 
	std::string source;					// links to the tile set used to create map 
};



