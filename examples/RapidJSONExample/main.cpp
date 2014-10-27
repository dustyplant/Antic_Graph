#include <rapidjson/reader.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/error/en.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>

#include <iostream>
#include <vector>
#include <cctype>
#include <sstream>
#include <fstream>

int main( int argc, char* argv[] )
{
	std::stringstream ss;
	std::ifstream ifs;
	ifs.open("Test.json", std::ios::binary);
	if( ifs.is_open() == false )
	{
		printf("Error: Could not open the file.\n");
		exit(EXIT_FAILURE);
	}

	ss << ifs.rdbuf();
	std::string stuff = ss.str();
	std::cout << stuff << std::endl;
	ifs.close();

	rapidjson::Document doc;
	if( doc.Parse<0>(stuff.c_str()).HasParseError() )
	{
		printf("Error: Parse error\n");
		exit( EXIT_FAILURE ); 
	}

	rapidjson::Value &s = doc["name"];

	std::cout << "Hello World!" << std::endl;
	std::cout << s.GetString() << std::endl;

	rapidjson::Value &textures = doc["texture"];
	for( rapidjson::SizeType i = 0; i < textures.Size(); ++i )
	{
		const rapidjson::Value &tex = textures[i];
		printf("%s:\t%s\n", tex["name"].GetString(), tex["path"].GetString());
	}

	rapidjson::Value &sprites = doc["sprites"];
	for( rapidjson::SizeType i = 0; i < sprites.Size(); ++i )
	{
		const rapidjson::Value &sprite = sprites[i];

		printf("[%d, %d, %d, %d]\n",
			sprite[rapidjson::SizeType(0)].GetInt(),
			sprite[rapidjson::SizeType(1)].GetInt(),
			sprite[rapidjson::SizeType(2)].GetInt(),
			sprite[rapidjson::SizeType(3)].GetInt()
		);
	}
}