#ifndef SPRITE_H
#define SPRITE_H

#include <GL/glew.h>
#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <Antic/Antic_Graph.h>
#include <Antic/Texture.h>
#include <Antic/Shader.h>

namespace agraph
{
	/**
	 * @brief A class that holds the data for a single sprite in a Sprite Sheet object.
	 */
	class Sprite
	{
	public:
		Sprite( Texture* tex, Rect &clipData );
		Sprite( Texture* tex, GLfloat x, GLfloat y, GLfloat w, GLfloat h );
		virtual ~Sprite();
		virtual void render( GLfloat x = 0, GLfloat y = 0, Shader* shader = nullptr );
		virtual void setClipData( const Rect &newClipData );
		virtual void setClipData( GLfloat x, GLfloat y, GLfloat w, GLfloat h );
		virtual void setTexture( Texture* tex );
		virtual Texture* getTexture();
		virtual Rect getClipData();
		virtual void cleanup();

	protected:
		Rect clipData;
		Texture* texture      = nullptr;
		GLuint uvBufferID     = 0;
		GLuint vertexBufferID = 0;
	};


	/**
	 * @brief An object that holds the data for a sprite sheet image.
	 */
	class SpriteSheet
	{
	public:
		virtual ~SpriteSheet();
		virtual bool init( Texture* texture, std::vector< Rect > &clipData );
		virtual Texture* getTexture();
		virtual std::vector< Sprite* > getSprites();
		virtual Sprite* getSprite( int index );
		
		virtual void render( int index, GLfloat x = 0, GLfloat y = 0, Shader* shader = nullptr );
		virtual void cleanup();

	protected:

		std::vector< Sprite* > sprites;
		Texture* texture;

	};

	class SpriteSheetFixed: public SpriteSheet
	{
	public:
		virtual void generateSprites( Texture* tex, GLuint tileWidth, GLuint tileHeight, GLint spacingX = 0, GLint spacingY = 0, GLuint startOffsetX = 0, GLuint startOffsetY = 0 );
		virtual GLuint getTileWidth();
		virtual GLuint getTileHeight();
		virtual GLint  getSpacingX();
		virtual GLint  getSpacingY();
		virtual GLuint getStartOffsetX();
		virtual GLuint getStartOffsetY();

	protected:
		GLuint tileWidth  = 16;
		GLuint tileHeight = 16;

		GLint  spacingX = 0;
		GLint  spacingY = 0;

		GLuint startOffsetX = 0;
		GLuint startOffsetY = 0;
	};


	/**
	 * @brief A factory that produces SpriteSheet objects.
	 * @details Safely creates, returns and deallocates SpriteSheet objects created through this factory.
	 */
	class SpriteSheetFactory
	{
	public:
		static SpriteSheet* loadSS( std::string spriteSheetPath );
		static SpriteSheet* addSS( std::string ssName, SpriteSheet* ss );
		static SpriteSheet* getSS( std::string ssName );
		static void freeSS( std::string name );
		static void cleanup();

	protected:
		static std::map< std::string, SpriteSheet* > ssDict;
	};
}

#endif