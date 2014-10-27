#include <Antic/Sprite.h>

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

std::map< std::string, agraph::SpriteSheet* > agraph::SpriteSheetFactory::ssDict;

agraph::Sprite::Sprite( agraph::Texture* tex, agraph::Rect &clipData )
{
	setTexture( tex );
	setClipData( clipData );
}

agraph::Sprite::Sprite( agraph::Texture* tex, GLfloat x, GLfloat y, GLfloat w, GLfloat h )
{
	setTexture( tex );
	setClipData( x, y, w, h );
}

agraph::Sprite::~Sprite()
{
	cleanup();
}

void agraph::Sprite::render( GLfloat x, GLfloat y, agraph::Shader* shader )
{
	texture->render( x, y, shader, this->vertexBufferID, this->uvBufferID );
}

void agraph::Sprite::setClipData( const agraph::Rect &newClipData )
{
	setClipData( newClipData.x, newClipData.y, newClipData.w, newClipData.h );
}

void agraph::Sprite::setClipData( GLfloat x, GLfloat y, GLfloat w, GLfloat h )
{
	this->clipData.x = x;
	this->clipData.y = y;
	this->clipData.w = w;
	this->clipData.h = h;

	GLuint texWidth  = getTexture()->getWidth();
	GLuint texHeight = getTexture()->getHeight();

	GLuint screenWidth = agraph::getScreenWidth();
	GLuint screenHeight = agraph::getScreenHeight();

	GLfloat vbd[] = {
		  w, 0.f, 0.f,
		  w,   h, 0.f,
		0.f, 0.f, 0.f,
		0.f,   h, 0.f,
	};

	/*
	GLfloat vbd[] = {
		w/screenWidth, 		0.f, 			0.f,
		w/screenWidth,    	h/screenHeight, 0.f,
		0.f, 				0.f, 			0.f,
		0.f,   				h/screenHeight, 0.f,
	};
	*/

	GLfloat uvbd[] = {
		(x+w)/texWidth,  y   /texHeight, 0.f,
		(x+w)/texWidth, (y+h)/texHeight, 0.f,
		 x   /texWidth,  y   /texHeight, 0.f,
	     x   /texWidth,	(y+h)/texHeight, 0.f,
	};

	cleanup();

	glGenBuffers( 1, &vertexBufferID );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBufferID );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vbd), vbd, GL_STATIC_DRAW );
	
	glGenBuffers( 1, &uvBufferID );
	glBindBuffer( GL_ARRAY_BUFFER, uvBufferID );
	glBufferData( GL_ARRAY_BUFFER, sizeof(uvbd), uvbd, GL_STATIC_DRAW );
}

void agraph::Sprite::setTexture( agraph::Texture* tex )
{
	this->texture = tex;
}

agraph::Texture* agraph::Sprite::getTexture()
{
	return this->texture;
}

agraph::Rect agraph::Sprite::getClipData()
{
	return this->clipData;
}

void agraph::Sprite::cleanup()
{
	if( vertexBufferID != 0 )
	{
		glDeleteBuffers( 1, &vertexBufferID );
		vertexBufferID = 0;
	}

	if( uvBufferID != 0 )
	{
		glDeleteBuffers( 1, &uvBufferID );
		uvBufferID = 0;
	}
}



///////////////////
/// SpriteSheet ///
///////////////////

agraph::SpriteSheet::~SpriteSheet()
{
	cleanup();
}

bool agraph::SpriteSheet::init( std::string spriteSheetPath )
{
	cleanup();

	std::stringstream ss;
	std::ifstream ifs;
	ifs.open( spriteSheetPath.c_str(), std::ios::binary );
	if( ifs.is_open() == false )
	{
		printf("AGraph Error: Failed to open SpriteSheet at %s.\n", spriteSheetPath.c_str());
		ifs.close();
		return false;
	}

	ss << ifs.rdbuf();
	ifs.close();

	rapidjson::Document doc;
	if( doc.Parse<0>(ss.str().c_str()).HasParseError() )
	{
		printf("AGraph Error: Parse error.\n");
		return false;
	}

	rapidjson::Value &name = doc["name"];
	rapidjson::Value &texture = doc["texture"];
	std::string texName = texture["name"].GetString();

	agraph::Texture* tex = agraph::TextureFactory::loadTexture( texName, texture["path"].GetString() );
	if( tex == nullptr )
	{
		printf("AGraph Error: Failed to load texture %s from SpriteSheet %s", texName.c_str(), name.GetString());
		return false;
	}

	rapidjson::Value &sprites = doc["sprites"];
	for( rapidjson::SizeType i = 0; i < sprites.Size(); ++i )
	{
		const rapidjson::Value &sprite = sprites[i];

		agraph::Sprite* tempSprite = new agraph::Sprite(
			tex,
			(GLfloat)sprite[rapidjson::SizeType(0)].GetInt(),
			(GLfloat)sprite[rapidjson::SizeType(1)].GetInt(),
			(GLfloat)sprite[rapidjson::SizeType(2)].GetInt(),
			(GLfloat)sprite[rapidjson::SizeType(3)].GetInt()
		);

		this->sprites.push_back( tempSprite );
	}

	return true;
}

bool agraph::SpriteSheet::init( agraph::Texture* texture, const std::vector< agraph::Rect > &clipData )
{
	return true;
}

void agraph::SpriteSheet::render( int index )
{
	getSprites()[ index ]->render();
}

void agraph::SpriteSheet::cleanup()
{
	for( int i = 0; i < sprites.size(); ++i )
	{
		delete sprites[i];
		sprites[i] = nullptr;
	}

	sprites.clear();
}

agraph::Texture* agraph::SpriteSheet::getTexture()
{
	return this->texture;
}

std::vector< agraph::Sprite* > agraph::SpriteSheet::getSprites()
{
	return this->sprites;
}

agraph::Sprite* agraph::SpriteSheet::getSprite( int index )
{
	return getSprites()[ index ];
}


//////////////////////////
/// SpriteSheetFactory ///
//////////////////////////

// TODO: Not finished yet. Load the JSON file and read in the data.
agraph::SpriteSheet* agraph::SpriteSheetFactory::loadSS( std::string spriteSheetPath )
{
	std::stringstream ss;
	std::ifstream ifs;
	ifs.open( spriteSheetPath.c_str(), std::ios::binary );
	if( ifs.is_open() == false )
	{
		printf("AGraph Error: Failed to open SpriteSheet at %s.\n", spriteSheetPath.c_str());
		ifs.close();
		return nullptr;
	}

	ss << ifs.rdbuf();
	ifs.close();

	rapidjson::Document doc;
	if( doc.Parse<0>(ss.str().c_str()).HasParseError() )
	{
		printf("AGraph Error: Failed to parse SpriteSheet at %s.\n", spriteSheetPath.c_str());
		return nullptr;
	}

	std::string name = doc["name"].GetString();

	if( ssDict.find( name ) != ssDict.end() )
		return ssDict[ name ];

	agraph::SpriteSheet* spriteSheet = new agraph::SpriteSheet;
	if( spriteSheet->init( spriteSheetPath ) == false )
	{
		delete spriteSheet;
		spriteSheet = nullptr;
	}

	return spriteSheet;
}

agraph::SpriteSheet* agraph::SpriteSheetFactory::getSS( std::string ssName )
{
	if( ssDict.find( ssName ) != ssDict.end() )
		return ssDict[ ssName ];

	return nullptr;
}

void agraph::SpriteSheetFactory::freeSS( std::string name )
{
	auto iter = ssDict.find( name );
	if( iter != ssDict.end() )
	{
		delete ssDict[ name ];
		ssDict.erase( iter );
	}
}

void agraph::SpriteSheetFactory::cleanup()
{
	for( auto i: ssDict )
		if( i.second != nullptr )
			delete i.second;

	if( ssDict.empty() == false )
		ssDict.clear();
}