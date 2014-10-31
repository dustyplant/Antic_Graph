#ifndef JSONUTILS_H
#define JSONUTILS_H

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

namespace antic
{
	rapidjson::Document* parseJSON( std::string filePath );
}

#endif