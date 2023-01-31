#include "FileImport.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include <fstream>

using namespace rapidjson;
using namespace std;


void FileImport::FileParse()
{

}

int main()
{
	Document document;
	const char* json = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";

	if (document.Parse(json).HasParseError())			// json file being parsed needs to passed as argument 
	{
		return 1;
		printf("no work!!!");
	} 
};