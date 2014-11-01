#include <Antic/Sprite.h>
#include <Antic/JSONUtils.h>
#include <iostream>
#include <vector>


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

	GLfloat w2 = w/2.f;
	GLfloat h2 = h/2.f;

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

bool agraph::SpriteSheet::init( agraph::Texture* texture, std::vector< agraph::Rect > &clipData )
{
	for( int i = 0; i < clipData.size(); ++i )
		sprites.push_back( new agraph::Sprite( texture, clipData[i] ) );

	this->texture = texture;

	return true;
}

void agraph::SpriteSheet::render( int index, GLfloat x, GLfloat y, Shader* shader )
{
	getSprites()[ index ]->render( x, y, shader );
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


////////////////////////
/// SpriteSheetFixed ///
////////////////////////

void agraph::SpriteSheetFixed::generateSprites( agraph::Texture* tex, GLuint tileWidth, GLuint tileHeight, GLint spacingX, GLint spacingY, GLuint startOffsetX, GLuint startOffsetY )
{
	cleanup();

	this->tileWidth    = tileWidth;
	this->tileHeight   = tileHeight;
	this->spacingX     = spacingX;
	this->spacingY     = spacingY;
	this->startOffsetX = startOffsetX;
	this->startOffsetY = startOffsetY;

	GLuint texWidth  = tex->getWidth();
	GLuint texHeight = tex->getHeight();
	int columns = ( texWidth  - startOffsetX ) / (GLfloat)( getTileWidth()  + spacingX );
	int rows    = ( texHeight - startOffsetY ) / (GLfloat)( getTileHeight() + spacingY );
	for( int j = 0; j < rows; ++j )
	{
		for( int i = 0; i < columns; ++i )
		{
			sprites.push_back(new Sprite(
				tex,
				i * tileWidth  + i * spacingX + startOffsetX,
				j * tileHeight + j * spacingY + startOffsetY,	
				getTileWidth(),
				getTileHeight()
			));
		}
	}

	this->texture = tex;
}

GLuint agraph::SpriteSheetFixed::getTileWidth()
{
	return this->tileWidth;
}

GLuint agraph::SpriteSheetFixed::getTileHeight()
{
	return this->tileHeight;
}

GLint  agraph::SpriteSheetFixed::getSpacingX()
{
	return this->spacingX;
}

GLint  agraph::SpriteSheetFixed::getSpacingY()
{
	return this->spacingY;
}

GLuint agraph::SpriteSheetFixed::getStartOffsetX()
{
	return this->startOffsetX;
}

GLuint agraph::SpriteSheetFixed::getStartOffsetY()
{
	return this->startOffsetY;
}


//////////////////////////
/// SpriteSheetFactory ///
//////////////////////////

agraph::SpriteSheet* agraph::SpriteSheetFactory::loadSS( std::string spriteSheetPath )
{
	rapidjson::Document* doc = antic::parseJSON( spriteSheetPath );

	rapidjson::Value &name = (*doc)["name"];
	if( ssDict.find( name.GetString() ) != ssDict.end() )
		return ssDict[ name.GetString() ];

	agraph::SpriteSheet* spriteSheet = nullptr;;

	rapidjson::Value &texture = (*doc)["texture"];
	std::string texName = texture["name"].GetString();

	agraph::Texture* tex = agraph::TextureFactory::loadTexture( texName, texture["path"].GetString() );
	if( tex == nullptr )
	{
		printf("AGraph Error: Failed to load texture %s from SpriteSheet %s", texName.c_str(), name.GetString());
		return nullptr;
	}


	rapidjson::Value &sprites = (*doc)["sprites"];
	if( sprites.IsArray() )
	{
		spriteSheet = new agraph::SpriteSheet;
		std::vector< agraph::Rect > clipData;

		for( rapidjson::SizeType i = 0; i < sprites.Size(); ++i )
		{
			const rapidjson::Value &sprite = sprites[i];
			agraph::Rect tempRect = {
				(GLfloat)sprite[rapidjson::SizeType(0)].GetInt(),
				(GLfloat)sprite[rapidjson::SizeType(1)].GetInt(),
				(GLfloat)sprite[rapidjson::SizeType(2)].GetInt(),
				(GLfloat)sprite[rapidjson::SizeType(3)].GetInt()
			};

			clipData.push_back( tempRect );
		}

		if( spriteSheet->init(tex, clipData) == false )
		{
			delete spriteSheet;
			spriteSheet = nullptr;
		}
	}
	else if( sprites.IsObject() )
	{
		GLuint tileWidth, tileHeight;

		if( sprites.HasMember("tileWidth") && sprites.HasMember("tileHeight") )
		{
			tileWidth  = sprites["tileWidth"].GetInt();
			tileHeight = sprites["tileHeight"].GetInt();			
			
			GLint  spacingX=0, spacingY=0;
			if( sprites.HasMember("spacingX") && sprites.HasMember("spacingY") )
			{
				spacingX = sprites["spacingX"].GetInt();
				spacingY = sprites["spacingY"].GetInt();
			}
			
			agraph::SpriteSheetFixed* ssf = new agraph::SpriteSheetFixed;
			ssf->generateSprites( tex, tileWidth, tileHeight, spacingX, spacingY );
			spriteSheet = ssf;
		}
	}

	delete doc;

	return spriteSheet;
}

agraph::SpriteSheet* agraph::SpriteSheetFactory::addSS( std::string ssName, agraph::SpriteSheet* ss )
{
	if( getSS( ssName ) == nullptr )
		ssDict[ ssName ] = ss;

	return getSS( ssName );
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