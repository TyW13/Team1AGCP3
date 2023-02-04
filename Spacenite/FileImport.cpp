#include "FileImport.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <fstream>

using namespace rapidjson;
using namespace std;


void FileImport::FileParse()
{
	Document document;

	FILE* filePointer = fopen("testMap.json", "rb");

	char readBuffer[5000];
	FileReadStream is(filePointer, readBuffer, sizeof(readBuffer));

	document.ParseStream(is);

	GenericArray layerArray = document["layers"].GetArray();

	for (Value::ConstValueIterator i = layerArray.Begin(); i < layerArray.End(); i++)
	{
		if (i->IsArray() == true)
		{
			auto obj = i->GetArray();
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