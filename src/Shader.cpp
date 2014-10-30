#include <Antic/Shader.h>
#include <fstream>
#include <algorithm>


std::map< std::string, agraph::Shader* > agraph::ShaderFactory::shaderDict;
std::vector< agraph::Shader* > 			 agraph::ShaderFactory::shaderList;

agraph::Shader::~Shader()
{
	cleanup();
}

GLuint agraph::Shader::getProgramID()
{
	return this->programID;
}

void agraph::Shader::setProgramID( GLuint programID )
{
	this->programID = programID;
}

void agraph::Shader::bind()
{
	glUseProgram( getProgramID() );
}

void agraph::Shader::cleanup()
{
	if( programID != 0 )
	{
		glDeleteProgram( programID );
		programID = 0;
	}
}

///////////////////
// ShaderFactory //
///////////////////

GLuint agraph::ShaderFactory::loadShaderSingle( GLenum shaderType, const std::string &shaderSource )
{
	GLint success = GL_FALSE;

	GLuint shaderID = glCreateShader( shaderType );
	const char *shaderPointer = shaderSource.c_str();
	glShaderSource( shaderID, 1, &shaderPointer, nullptr );
	glCompileShader( shaderID );

	glGetShaderiv( shaderID, GL_COMPILE_STATUS, &success );
	if( success == GL_FALSE )
	{
		GLint maxLength = 0;
		glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &maxLength );
		std::vector<GLchar> errorLog( maxLength );
		glGetShaderInfoLog( shaderID, maxLength, &maxLength, &errorLog[0] );

		fprintf( stdout, "%s\n", &errorLog[0] );
		glDeleteShader( shaderID );
		shaderID = 0;
	}

	return shaderID;
}

GLuint agraph::ShaderFactory::linkShaders( GLuint vertexShaderID, GLuint fragmentShaderID )
{
	GLuint programID = glCreateProgram();
	glAttachShader( programID, vertexShaderID );
	glAttachShader( programID, fragmentShaderID );
	glLinkProgram( programID );

	GLint success;
	glGetProgramiv( programID, GL_LINK_STATUS, &success );
	if( success == GL_FALSE ) {
		GLint maxLength;
		glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &maxLength );
		std::vector<char> infoLog( maxLength );
		glGetShaderInfoLog( programID, maxLength, nullptr, &infoLog[0] );
		fprintf( stdout, "%s\n", &infoLog[0] );

		glDeleteProgram( programID );
	}

	glDeleteShader( vertexShaderID );
	glDeleteShader( fragmentShaderID );

	return programID;
}

std::string agraph::ShaderFactory::getFileSource( const std::string &filePath )
{
	std::string shaderCode;
    std::ifstream shaderStream(filePath.c_str(), std::ios::in);
    if(shaderStream.is_open())
    {
        std::string Line = "";
        while(getline(shaderStream, Line))
            shaderCode += "\n" + Line;
        shaderStream.close();
    }

    return shaderCode;
}

agraph::Shader* agraph::ShaderFactory::loadShadersString( const std::string &vertexShaderSource, const std::string &fragmentShaderSource )
{
	GLuint vertexShaderID = loadShaderSingle( GL_VERTEX_SHADER, vertexShaderSource );
	GLuint fragmentShaderID = loadShaderSingle( GL_FRAGMENT_SHADER, fragmentShaderSource );

	GLuint programID = 0;
	if( vertexShaderID != 0 && fragmentShaderID != 0 )
		programID = linkShaders( vertexShaderID, fragmentShaderID );

	// Adds shader program id if the shader was loaded without error.
	agraph::Shader* shader = nullptr;
	if( programID != 0 )
	{
		shader = new agraph::Shader;
		shader->setProgramID( programID );
	}

	return shader;
}

agraph::Shader* agraph::ShaderFactory::loadShaders( const std::string &vertexFile, const std::string &fragmentFile )
{
	return loadShadersString( getFileSource(vertexFile), getFileSource(fragmentFile) );
}

agraph::Shader* agraph::ShaderFactory::loadShadersString( const std::string &name, const std::string &vertexShaderSource, const std::string &fragmentShaderSource )
{
	if( shaderDict.find(name) == shaderDict.end() )
		shaderDict[ name ] = loadShadersString( vertexShaderSource, fragmentShaderSource );

	return shaderDict[ name ];
}

agraph::Shader* agraph::ShaderFactory::loadShaders( const std::string &name, const std::string &vertexFile, const std::string &fragmentFile )
{
	if( shaderDict.find(name) == shaderDict.end() )
		shaderDict[ name ] = loadShaders( vertexFile, fragmentFile );

	return shaderDict[ name ];
}

agraph::Shader* agraph::ShaderFactory::getShader( std::string name )
{
	// Return Shader only if found in map, otherwise return nullptr.
	if( shaderDict.find( name ) != shaderDict.end() )
		return shaderDict[ name ];

	return nullptr;
}

void agraph::ShaderFactory::freeShader( std::string name )
{
	auto iter = shaderDict.find( name );
	if( iter != shaderDict.end() )
	{
		delete shaderDict[ name ];
		shaderDict.erase( iter );
	}
}

void agraph::ShaderFactory::cleanup()
{
	for( auto i: shaderDict )
		if( i.second != nullptr )
			delete i.second;

	for( auto i: shaderList )
		if( i != nullptr )
			delete i;

	if( shaderDict.empty() == false )
		shaderDict.clear();

	if( shaderList.empty() == false )
		shaderList.clear();
}