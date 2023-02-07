#include "Map.h"
#include "rapidjson/document.h"

using namespace rapidjson;
using namespace std;

Map::Map(const char* mapName)
{
	document.Parse(mapName);

	// Stores all the data from the json file in the respective variables from the class

	height = document["height"].GetInt();
	infinite = document["infinite"].GetBool();			

	nextlayerid = document["nextlayerid"].GetInt();
	nextobjectid = document["nextobjectid"].GetInt();
	orientation = document["orientation"].GetString();
	renderorder = document["renderorder"].GetString();
	tiledversion = document["tiledversion"].GetString();
	tileheight = document["tileheight"].GetInt();

	rapidjson::Value::Array layersArray = document["layers"].GetArray();
	for (size_t i = 0; i < layersArray.Capacity(); i++)
	{
		Layers layer(layersArray[i]);
		layers.push_back(layer);
	}

	tilewidth = document["tilewidth"].GetInt();
	type = document["type"].GetString();
	version = document["version"].GetFloat();
	width = document["width"].GetInt();

	rapidjson::Value::Array tileArray = document["tilesets"].GetArray();
	for (size_t i = 0; i < tileArray.Capacity(); i++)
	{
		TileSetMap tileset(tileArray[i]);
		tilesets.push_back(tileset);
	}
}

Map::~Map()
{

}

Layers::Layers(rapidjson::Value& value)
{
	if (value.HasMember("data"))
	{
		for (auto& v : value.GetArray()) 
		{
			data.push_back(v.GetInt());
		}
	}
}

Layers::~Layers()
{
}
