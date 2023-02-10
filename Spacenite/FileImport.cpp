#include "FileImport.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <fstream>

using namespace rapidjson;
using namespace std;


void FileParse()
{
	FILE* filePointer = fopen("data/test_level_jump1.json", "rb");		// Opens json file via pointer

	char readBuffer[5000];		// User provided buffer
	FileReadStream is(filePointer, readBuffer, sizeof(readBuffer));

	Document document;				
	document.ParseStream(is);

	GenericArray layerArray = document["layers"].GetArray();

	for (Value::ConstValueIterator i = layerArray.Begin(); i < layerArray.End(); i++)
	{
		if (i->IsArray() == true)
		{
			GenericArray dataArray = i->GetArray();
		}
		

		//if (obj.HasMember("data"))
		//{
		//	auto data = obj.FindMember("data");

		//	auto const& LayerData = data->value;

		//	for (auto& v: LayerData.GetArray()) 
		//	{
		//		printf(" %d", v.GetInt());
		//	}
		//}
	}

	fclose(filePointer);
}
