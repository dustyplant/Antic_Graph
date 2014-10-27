#include <Antic/Texture.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <glm/glm.hpp>
#include <Antic/Antic_Graph.h>


std::string VertexShaderDefault = "#version 120\n"
	"attribute vec3 vertexPosition;"
	"attribute vec2 vertexUV;"
	"varying vec2 UV;"
	"uniform mat4 MVP;"
	"void main() {"
		"gl_Position = MVP * vec4( vertexPosition, 1 );"
		"UV = vertexUV;"
	"}";

std::string FragmentShaderDefault = "#version 120\n"
	"varying vec2 UV;"
	"uniform sampler2D textureSampler;"
	"void main() {"
		"gl_FragColor = texture2D( textureSampler, UV );"
	"}";

GLuint defaultTextureShader = 0;

agraph::Texture::Texture()
{
	textureID      = 0;
	textureWidth   = 0;
	textureHeight  = 0;
	vertexBufferID = 0;
	uvBufferID     = 0;
}
agraph::Texture::~Texture()
{
	cleanup();
}

bool agraph::Texture::init( std::string texturePath )
{
	// Cleanup the texture in case it was already initialized.
	cleanup();
	if( defaultTextureShader == 0 )
		 defaultTextureShader = agraph::ShaderFactory::loadShadersString( "Default Texture", VertexShaderDefault, FragmentShaderDefault )->getProgramID();

	ILuint imageID;
	ilGenImages( 1, &imageID );
	ilBindImage( imageID );
	ILboolean success = ilLoadImage( texturePath.c_str() );
	if( success == IL_TRUE )
	{
		success = ilConvertImage( IL_RGBA, IL_UNSIGNED_BYTE );
		if( success == IL_TRUE )
		{
			// Generate and bind the texture.
			glGenTextures( 1, &textureID );
			glBindTexture( GL_TEXTURE_2D, textureID );

			setWidth(  (GLuint)ilGetInteger( IL_IMAGE_WIDTH ) );
			setHeight( (GLuint)ilGetInteger( IL_IMAGE_HEIGHT ) );
			
			iluImageParameter( ILU_PLACEMENT, ILU_UPPER_LEFT );

			// Pass the texture data obtained by DevIL to OpenGL to generate the texture.
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA8,
				getWidth(),
				getHeight(),
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				(GLuint*)ilGetData()
			);


			// This determines what happens when the texture is scaled.
			// GL_LINEAR gets the average of the 4 closest pixels to approximate the proper color.
			// GL_NEAREST just picks the closest pixel to that location and uses that. Not particularly faster. Not worth using.
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

			// This describes what is drawn if the image is too large.
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
		}

		// Free the DevIL image id. The data is now stored in textureID and we no longer need imageID.
		ilDeleteImages( 1, &imageID );
	}

	if( success == IL_FALSE )
	{
		printf("Error Loading: %s\n", texturePath.c_str());
		cleanup();
	}
	else
	{
		// If no errors loading texture, generate the VBOs.
		setBuffers();

		glGenBuffers( 1, &vertexBufferID );
		glBindBuffer( GL_ARRAY_BUFFER, vertexBufferID );
		glBufferData(
			GL_ARRAY_BUFFER,
			vertexBufferData.size() * sizeof( GLfloat ),
			&vertexBufferData[0],
			GL_STATIC_DRAW
		);

		glGenBuffers( 1, &uvBufferID );
		glBindBuffer( GL_ARRAY_BUFFER, uvBufferID );
		glBufferData(
			GL_ARRAY_BUFFER,
			uvBufferData.size() * sizeof( GL_FLOAT ),
			&uvBufferData[0],
			GL_STATIC_DRAW
		);

	}

	return success == IL_TRUE;
}

void agraph::Texture::cleanup()
{
	if( textureID != 0 )
	{
		glDeleteTextures( 1, &textureID );
		textureID = 0;
	}

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

	// Clear the buffer data as well.
	vertexBufferData.clear();
		uvBufferData.clear();
}

void agraph::Texture::bind()
{
	glBindTexture( GL_TEXTURE_2D, getTextureID() );
}

GLuint agraph::Texture::getWidth()
{
	return this->textureWidth;
}

GLuint agraph::Texture::getHeight()
{
	return this->textureHeight;
}

GLuint agraph::Texture::getTextureID()
{
	return this->textureID;
}

void agraph::Texture::render()
{
	agraph::Texture::render( 0.0f, 0.0f, (GLuint)0 );
}

void agraph::Texture::render( GLfloat x, GLfloat y, agraph::Shader* shader, GLuint vertexBuffer, GLuint uvBuffer )
{
	GLuint programID = 0;
	if( shader != nullptr )
		programID = shader->getProgramID();

	agraph::Texture::render( x, y, programID, vertexBuffer, uvBuffer );
}

void agraph::Texture::render( GLfloat x, GLfloat y, GLuint programID, GLuint vertexBuffer, GLuint uvBuffer )
{
	// If no programID given, or one with value 0, it will use the default for textures.
	if( programID == 0 )
		programID = defaultTextureShader;

	if( vertexBuffer == 0 )
		vertexBuffer = this->vertexBufferID;

	if( uvBuffer == 0 )
		uvBuffer = this->uvBufferID;

	// Set the shader to use.
	glUseProgram( programID );


	// Get attribute and uniform location values.
	GLuint vertexPositionID = glGetAttribLocation( programID, "vertexPosition" );
	GLuint vertexUVID       = glGetAttribLocation( programID, "vertexUV" );
	
	GLuint MatrixID         = glGetUniformLocation( programID, "MVP" );
	GLuint TextureSamplerID = glGetUniformLocation( programID, "textureSampler" );

	//glm::mat4 MVP = agraph::ProjectionOrtho * agraph::View * agraph::Model;
	glm::mat4 MVP = agraph::Projection * agraph::View * agraph::Model;

	// Pass the MVP matrix data to the shaders.
	glUniformMatrix4fv( MatrixID, 1, GL_FALSE, &MVP[0][0] );

	// Bind this texture and pass it's data to the texture sampler.
	glActiveTexture( GL_TEXTURE0 );
	this->bind();
	glUniform1i( TextureSamplerID, 0 );

	// Pass in the vertex data.
	glEnableVertexAttribArray( vertexPositionID );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glVertexAttribPointer( vertexPositionID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	// Pass the uv data to the shader.
	glEnableVertexAttribArray( vertexUVID );
	glBindBuffer( GL_ARRAY_BUFFER, uvBuffer );
	glVertexAttribPointer( vertexUVID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	// This is where you actually render the texture. 12 is 3 GLfloats times 4 vertices.
	glDrawArrays( GL_TRIANGLE_STRIP, 0, vertexBufferData.size() );

	// Disable the Vertex Attribute Arrays because we are done with them now.
	glDisableVertexAttribArray( vertexPositionID );
	glDisableVertexAttribArray( vertexUVID );
}

void agraph::Texture::setBuffers()
{
	GLfloat w2 = (GLfloat)getWidth()/2.f;
	GLfloat h2 = (GLfloat)getHeight()/2.f;

	GLuint screenWidth = agraph::getScreenWidth();
	GLuint screenHeight = agraph::getScreenHeight();

	GLfloat vbd[] = {
		(GLfloat)textureWidth, 0.f, 0.f,
		(GLfloat)textureWidth, (GLfloat)textureHeight, 0.f,
		0.f,  0.f, 0.f,
		0.f, (GLfloat)textureHeight, 0.f,
	};

	/*
	// For use with textures enlarged to power of 2 sizes.
	static GLfloat uvbd[] = {
		(GLfloat)imageWidth/textureWidth, 0.f, 0.f,
		(GLfloat)imageWidth/textureWidth, (GLfloat)imageHeight/textureHeight, 0.f,
		0.f, 0.f, 0.f,
	    0.f, (GLfloat)imageHeight/textureHeight, 0.f,
	};
	*/

	static GLfloat uvbd[] = {
		1.f, 0.f, 0.f,
		1.f, 1.f, 0.f,
		0.f, 0.f, 0.f,
	    0.f, 1.f, 0.f,
	};

	vertexBufferData.clear();
	vertexBufferData.insert( vertexBufferData.end(), vbd, vbd +sizeof(vbd)/sizeof(GLfloat) );

	uvBufferData.clear();
	uvBufferData.insert( uvBufferData.end(), uvbd, uvbd +sizeof(uvbd)/sizeof(GLfloat) );
}

void agraph::Texture::setWidth( GLuint width )
{
	this->textureWidth = width;
}

void agraph::Texture::setHeight( GLuint height )
{
	this->textureHeight = height;
}



//////////////////////
/// TextureFactory ///
//////////////////////


std::map< std::string, agraph::Texture* > agraph::TextureFactory::textureDict;
std::vector< agraph::Texture* > agraph::TextureFactory::textureList;

agraph::Texture* agraph::TextureFactory::loadTexture( std::string name, std::string texturePath )
{
	if( textureDict.find( name ) != textureDict.end() )
		return textureDict[ name ];

	agraph::Texture* tex = new Texture;
	if( tex->init( texturePath ) == false )
	{
		tex->cleanup();
		delete tex;
		return nullptr;
	}

	textureDict[ name ] = tex;
	return tex;
}

agraph::Texture* agraph::TextureFactory::loadTexture( std::string texturePath )
{
	agraph::Texture* tex = new Texture;
	if( tex->init( texturePath ) == false )
	{
		tex->cleanup();
		delete tex;
		return nullptr;
	}

	textureList.push_back( tex );
	return tex;
}

agraph::Texture* agraph::TextureFactory::getTexture( std::string name )
{
	// Return texture only if found in map, otherwise return nullptr.
	if( textureDict.find( name ) != textureDict.end() )
		return textureDict[ name ];

	return nullptr;
}

void agraph::TextureFactory::freeTexture( std::string name )
{
	auto iter = textureDict.find( name );
	if( iter != textureDict.end() )
	{
		delete textureDict[ name ];
		textureDict.erase( iter );
	}
}

void agraph::TextureFactory::cleanup()
{
	// Free all Texture objects stored in the map of textures.
	for( auto i: textureDict )
		if( i.second->getTextureID() != 0 )
			delete i.second;

	// Free all Texture objects stored in the vector of textures.
	for( auto i: textureList )
		if( i->getTextureID() != 0 )
			delete i;

	if( textureDict.empty() == false )
		textureDict.clear();
	
	if( textureList.empty() == false )
		textureList.clear();
}