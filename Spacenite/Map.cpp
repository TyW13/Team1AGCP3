#include "Map.h"

using namespace rapidjson;
using namespace std;

Map::Map()
{
	FILE* fp = fopen("data/test_level_jump1.json", "rb");		// opens json file 

	char readBuffer[10000];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document document;
	document.ParseStream(is);			// parses json file 

	fclose(fp);		// closes json once it has been read 

	// Stores all the data from the json file in the respective variables from the class

	height = document["height"].GetInt();
	infinite = document["infinite"].GetBool();

	rapidjson::GenericArray layersArray = document["layers"].GetArray();
	rapidjson::GenericObject layersObj = layersArray.begin()->GetObject();
	rapidjson::GenericArray dataArray = layersObj["data"].GetArray();
	
	Value::Array layersA = document["layers"].GetArray();
	for (size_t i = 0; i < layersA.Capacity(); i++)
	{
		rapidjson::Value layerValue = layersA[0].GetArray();						////// FIX THIS LOOP STUFF ////
		if (layersA[0].HasMember("data"))
		{
			for (auto& v : layerValue["data"].GetArray())
			{
				data.push_back(v.GetInt());
			}
		}
		//Layer layer(layersA[i]);
		//layers.push_back(layer);
	}

	name = layersObj["name"].GetString();
	id = layersObj["id"].GetInt();
	opacity = layersObj["opacity"].GetInt();
	layersType = layersObj["type"].GetString();
	visible = layersObj["visible"].GetBool();
	x = layersObj["x"].GetInt();
	y = layersObj["y"].GetInt();

	nextlayerid = document["nextlayerid"].GetInt();
	nextobjectid = document["nextobjectid"].GetInt();
	orientation = document["orientation"].GetString();
	renderorder = document["renderorder"].GetString();
	tiledversion = document["tiledversion"].GetString();
	tileheight = document["tileheight"].GetInt();

	rapidjson::Value::Array tileArray = document["tilesets"].GetArray();
	rapidjson::GenericObject tileObj = tileArray.begin()->GetObject();
	firstgid = tileObj["firstgid"].GetInt();
	source = tileObj["source"].GetString();

	tilewidth = document["tilewidth"].GetInt();
	type = document["type"].GetString();
	width = document["width"].GetInt();
}

void TileSet::Init(Document& tilesetDoc)
{
	columns = tilesetDoc["columns"].GetInt();
	image = tilesetDoc["image"].GetString();
	imageHeight = tilesetDoc["imageheight"].GetInt();
	imageWidth = tilesetDoc["imagewidth"].GetInt();
	margin = tilesetDoc["margin"].GetInt();
	name = tilesetDoc["name"].GetString();
	spacing = tilesetDoc["spacing"].GetInt();
	tilecount = tilesetDoc["tilecount"].GetInt();
	tileheight = tilesetDoc["tileheight"].GetInt();
	tilewidth = tilesetDoc["tilewidth"].GetInt();
	type = tilesetDoc["type"].GetString();
}
