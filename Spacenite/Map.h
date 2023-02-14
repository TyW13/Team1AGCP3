#pragma once
#include <string>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

class Map
{
public:

	Map::Map();

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

	std::vector<int> data;				// Data from layers data array
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

class TileSet
{
public:
	TileSet::TileSet(){}
//	TileSet::TileSet(int firstgid, const char* tileset);
	~TileSet() {}

	void Init(rapidjson::Document& tilesetDoc);
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
