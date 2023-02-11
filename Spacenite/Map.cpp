#include "Map.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;
using namespace std;

Map::Map()
{
	FILE* fp = fopen("data/test_level_jump1.json", "rb");		// opens json file 

	char readBuffer[10000];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document document;
	document.ParseStream(is);			// parses json file 

	fclose(fp);		// closes json once it has been read 

	// Stores all the data from the json file in the respective variables from the class

	height = document["height"].GetInt();
	infinite = document["infinite"].GetBool();	

	GenericArray layersArray = document["layers"].GetArray();									// Gets everything in the layers array in json and stores in new 
	GenericObject layersObj = layersArray.begin()->GetObject();
	GenericArray dataArray = layersObj["data"].GetArray();
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
	GenericObject tileObj = tileArray.begin()->GetObject();
	firstgid = tileObj["firstgid"].GetInt();
	source = tileObj["source"].GetString();

	tilewidth = document["tilewidth"].GetInt();
	type = document["type"].GetString();
	width = document["width"].GetInt();
}

//Layers::Layers(rapidjson::Value& value)			
//{
//	if (value.HasMember("data"))
//	{
//		for (auto& v : value.GetArray()) 
//		{
//			data.push_back(v.GetInt());
//		}
//	}
//	
//	if (value.HasMember("height"))
//	{
//		height = value["height"].GetInt();
//	}
//
//	id = value["id"].GetInt();
//
//	if (value.HasMember("image")) 
//	{
//		image = value["image"].GetString();
//	}
//
//	name = value["name"].GetString();
//	opacity = value["opacity"].GetInt();
//	type = value["type"].GetString();
//	visible = value["visible"].GetBool();
//
//	if (value.HasMember("width")) 
//	{
//		width = value["width"].GetInt();
//	}
//
//	x = value["x"].GetInt();
//	y = value["y"].GetInt();
//}

//TileSetMap::TileSetMap(rapidjson::Value& value) 
//{
//	firstgid = value["firstgid"].GetInt();
//	source = value["source"].GetString();
//}
