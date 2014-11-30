#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <map>
#include <Antic/Shader.h>

namespace agraph
{
	/**
	 * @brief Stores the data for a given texture or image.
	 */
	class Texture
	{
	public:
		Texture();
		virtual ~Texture();

		virtual bool init( std::string texturePath );
		void bind();
		GLuint getWidth();
		GLuint getHeight();

		GLuint getTextureID();

		virtual void render();
		virtual void render( GLfloat x, GLfloat y, GLuint programID, GLuint vertexBuffer = 0, GLuint uvBuffer = 0 );
		virtual void render( GLfloat x, GLfloat y, Shader* shader = nullptr, GLuint vertexBuffer = 0, GLuint uvBuffer = 0 );

		void cleanup();

	protected:
		GLuint textureID;

		GLuint textureWidth;
		GLuint textureHeight;


		std::vector< GLfloat > vertexBufferData;
		std::vector< GLfloat >     uvBufferData;

		//GLuint vertexArrayID  = 0;
		GLuint vertexBufferID = 0;
		GLuint uvBufferID     = 0;

		GLuint* pixelData;

	private:
		void setBuffers();
		void setWidth( GLuint width );
		void setHeight( GLuint height );
		/*
		void setTextureWidth( GLuint width );
		void setTextureHeight( GLuint height );
		*/
	};


	/**
	 * @brief A factory that produces Texture objets.
	 * @details Safely creates, returns and deallocates Texture objects created through this factory.
	 */
	class TextureFactory
	{
	public:
		static Texture* loadTexture( std::string name, std::string texturePath );
		static Texture* loadTexture( std::string texturePath );
		static Texture*  getTexture( std::string name );
		static void		freeTexture( std::string name );
		static void	 		cleanup();

	protected:
		static std::map< std::string, Texture* > textureDict;
		static std::vector< Texture* > textureList;
	};
}

#endif