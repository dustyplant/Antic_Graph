#include <Antic/JSONUtils.h>

rapidjson::Document* antic::parseJSON( std::string filePath )
{
	std::stringstream ss;
	std::ifstream ifs;
	ifs.open( filePath.c_str(), std::ios::binary );
	if( ifs.is_open() == false )
	{
		printf("JSONUtil Error: Failed to open JSON file at %s.\n",filePath.c_str());
		ifs.close();
		return nullptr;
	}

	ss << ifs.rdbuf();
	ifs.close();

	rapidjson::Document *doc = new rapidjson::Document;
	if( doc->Parse<0>(ss.str().c_str()).HasParseError() )
	{
		printf("JSONUtil Error: Parse error.\n");
		return nullptr;
	}

	return doc;
}