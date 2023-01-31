#include "FileImport.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include <fstream>

using namespace rapidjson;
using namespace std;


inline const char* FileImport::FileParse()
{
	json = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";

	if (document.Parse(json).HasParseError())			// json file being parsed needs to passed as argument 
	{
		printf("why no work!!!");
	}

	FILE* filePointer = fopen("testMap.json", "rb");
	size_t size = ftell(filePointer);
	char* buffer = new char[size];
	fread(buffer, sizeof(char), size, filePointer);
	return buffer;
}

