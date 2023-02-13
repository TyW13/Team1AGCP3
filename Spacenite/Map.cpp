#include "Map.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;
using namespace std;

Map::Map()
{
	FILE* fp = fopen("data/test_level_jump1.json", "rb");		// opens json file 
	FILE* fp = fopen("data/TestingLevel0.json", "rb");		// opens json file 

	char readBuffer[10000];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document document;
	document.ParseStream(is);			// parses json file 

	fclose(fp);		// closes json once it has been read 

	// Stores all the data from the json file in the respective variables from the class

	height = document["height"].GetInt();
<<<<<<< Updated upstream
	infinite = document["infinite"].GetBool();

	GenericArray layersArray = document["layers"].GetArray();
	GenericObject layersObj = layersArray.begin()->GetObject();
	GenericArray dataArray = layersObj["data"].GetArray();
	name = layersObj["name"].GetString();
	id = layersObj["id"].GetInt();
	opacity = layersObj["opacity"].GetInt();
	layersType = layersObj["type"].GetString();
	visible = layersObj["visible"].GetBool();
	x = layersObj["x"].GetInt();
	y = layersObj["y"].GetInt();
=======
	infinite = document["infinite"].GetBool();			

	Value::Array layersA = document["layers"].GetArray();
	for (size_t i = 0; i < layersA.Capacity(); i++)
	{
		Layers layer(layersA[i]);
		layers.push_back(layer);
	}
	if (document["layers"].IsArray()) {
		printf("IS array");
	}
	
	
	Value::Array mapLayers = document["layers"].GetArray();
	Layers mapLayerArray = mapLayers[0];
	
	layers.push_back(mapLayerArray);
	
	Layers onelayer(document["layers"].GetArray()[0]);
>>>>>>> Stashed changes

	nextlayerid = document["nextlayerid"].GetInt();
	nextobjectid = document["nextobjectid"].GetInt();
	orientation = document["orientation"].GetString();
	renderorder = document["renderorder"].GetString();
	tiledversion = document["tiledversion"].GetString();
	tileheight = document["tileheight"].GetInt();

<<<<<<< Updated upstream
=======
	//GenericArray layersArray = document["layers"].GetArray();									// Gets everything in the layers array in json and stores in new 
	//GenericObject test = layersArray.begin()->GetObject();
	//GenericArray data = test["data"].GetArray();
	//int data1 = data[696].GetInt();
	//string floor = test["name"].GetString();
	//layers.push_back(layer);

>>>>>>> Stashed changes
	rapidjson::Value::Array tileArray = document["tilesets"].GetArray();
	GenericObject tileObj = tileArray.begin()->GetObject();
	firstgid = tileObj["firstgid"].GetInt();
	source = tileObj["source"].GetString();

	tilewidth = document["tilewidth"].GetInt();
	type = document["type"].GetString();
	width = document["width"].GetInt();
<<<<<<< Updated upstream
=======

	

	delete[](fp);
}

Layers::Layers(rapidjson::Value& value)			
{
	if (value.HasMember("data"))
	{
		for (auto& v : value["data"].GetArray())
		{
			data.push_back(v.GetInt());
		}
	}
	
	if (value.HasMember("height"))
	{
		height = value["height"].GetInt();
	}

	id = value["id"].GetInt();

	if (value.HasMember("image")) 
	{
		image = value["image"].GetString();
	}

	name = value["name"].GetString();
	opacity = value["opacity"].GetInt();
	type = value["type"].GetString();
	visible = value["visible"].GetBool();

	if (value.HasMember("width")) 
	{
		width = value["width"].GetInt();
	}

	x = value["x"].GetInt();
	y = value["y"].GetInt();
}

TileSetMap::TileSetMap(rapidjson::Value& value) 
{
	firstgid = value["firstgid"].GetInt();
	source = value["source"].GetString();
}


TileSet::TileSet(int firstgid, const char* tileset)
{
	FILE* f = fopen("data/TSTestingLevel0.json", "rb");		// opens json file 

	if (!f)
	{
		printf("didnt work");
	}

	char readBuffer[10000];
	FileReadStream is(f, readBuffer, sizeof(readBuffer));

	Document tilesetDoc;
	tilesetDoc.ParseStream(is);			// parses json file 

	fclose(f);

	//Document tilesetDoc;
	//tilesetDoc.Parse(tileset);

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

	delete[](tileset);
>>>>>>> Stashed changes
}
