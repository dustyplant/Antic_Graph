#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <vector>
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

namespace agraph
{
	class Shader
	{
	public:
		/**
		 * @brief Shader destructor that calls the cleanup function.
		 */
		virtual ~Shader();

		/**
		 * @brief Binds the current shader using the OpenGL command glUseProgram.
		 */
		virtual void bind();

		/**
		 * @return The program ID of Shader object.
		 */
		virtual GLuint getProgramID();

		/**
		 * @brief Sets the program ID of the Shader object.
		 * @details This is used to modify the value of the Shader. Note, this does not cleanup the previous shader program.
		 * 
		 * @param programID The program ID to give to the current Shader object.
		 */
		virtual void setProgramID( GLuint programID );

		/**
		 * @brief Cleans up the Shader object by deleting the OpenGL shader program that it wraps.
		 */
		virtual void cleanup();

	protected:

		/// Holds an ID of the OpenGL shader program. The Shader object is a wrapper for this.
		GLuint programID;
	};



	class ShaderFactory
	{
	public:
		/**
		 * @brief Loads a single shader program of the given shader type from the given shader source string.
		 * 
		 * @param shaderType A GLenum corresponding to the type of shader to be compiled. (i.e. GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
		 * @param shaderSource The source string of the shader to be compiled.
		 * 
		 * @return The ID of the compiled shader. This can be later linked in the linkShaders static function of the ShaderFactory.
		 */
		static GLuint loadShaderSingle( GLenum shaderType, const std::string &shaderSource );

		/**
		 * @brief Links shaders together into a shader program. Currently only supports a vertex shader and a fragment shader.
		 * 
		 * @param vertexShaderID The shader ID of the vertex shader.
		 * @param fragmentShaderID The shader ID of the fragment shader.
		 * 
		 * @return The ID of the compiled and linked shader program. This can be used with the OpenGL command glUseProgram(GLuint).
		 */
		static GLuint linkShaders( GLuint vertexShaderID, GLuint fragmentShaderID );

		/**
		 * @brief Loads the contents of a file into the a string.
		 * @details Used here to get the shader source code from a file to later be compiled and linked into a shader program with other ShaderFactory static functions.
		 * 
		 * @param filePath The path to the file to be read into a string.
		 * @return The contents of the file at the given file path.
		 */
		static std::string getFileSource( const std::string &filePath );
		
		/**
		 * @brief Loads a shader program from two source strings as opposed to two files. This version does not take a name.
		 * 
		 * @param vertexShaderSource The source string of the vertex shader source code.
		 * @param fragmentShaderSource The source string of the fragment shader source code.
		 * @return A pointer to the Shader object that was just loaded.
		 */
		static Shader* loadShadersString( const std::string &vertexShaderSource, const std::string &fragmentShaderSource );

		/**
		 * @brief Loads a shader program from two source strings as opposed to two files. This version does take a name.
		 * 
		 * @param name The name to be given to the Shader object. A pointer to the Shader object can be retrieved by passing this name to ShaderFactory's getShader static function.
		 * @param vertexShaderSource The source string of the vertex shader source code.
		 * @param fragmentShaderSource The source string of the fragment shader source code.
		 * @return A pointer to the Shader object that was just loaded.
		 */
		static Shader* loadShadersString( const std::string &name, const std::string &vertexShaderSource, const std::string &fragmentShaderSource );


		/**
		 * @brief Loads a shader program from two files as opposed to two source strings. This version doesn not take a name.
		 * 
		 * @param vertexFile The path to the file containing the vertex shader source code.
		 * @param fragmentFile The path to the file containing the fragment shader source code.
		 * @return A pointer to the Shader object that was just loaded.
		 */
		static Shader* loadShaders( const std::string &vertexFile, const std::string &fragmentFile );

		/**
		 * @brief Loads a shader program from two files as opposed to two source strings. This version takes a name.
		 * @param name The name to be given to the Shader object. A pointer to the Shader object can be retrieved by passing this name to ShaderFactory's getShader static function.
		 * 
		 * @param vertexFile The path to the file containing the vertex shader source code.
		 * @param fragmentFile The path to the file containing the fragment shader source code.
		 * @return A pointer to the Shader object that was just loaded.
		 */
		static Shader* loadShaders( const std::string &name, const std::string &vertexFile, const std::string &fragmentFile );


		/**
		 * @param name Name of the Shader object to be retrieved from the shaderDict hash map.
		 * @return The Shader object of the given name from shaderDict. If none is found returns nullptr (NULL).
		 */
		static Shader* getShader( std::string name );

		/**
		 * @brief Deallocates the shader of the given name if found in the shaderDict map. This also removes the Shader object and it's name from the map.
		 * 
		 * @param name The name of the shader to be freed and removed from the map.
		 */
		static void freeShader( std::string name );

		/**
		 * @brief Cleans up everything within the ShaderFactory.
		 * @details Frees and empties the shaderDict hash map of Shader objects and the shaderList vector of Shader objects.
		 */
		static void cleanup();

	protected:
		/// A map of string names to shader program ids. You can switch shaders with the command glUseProgram( agraph::shaders["MyShader"] );
		static std::map< std::string, Shader* > shaderDict;

		/// This is the list of program ids that are loaded without a name given. This is just for cleanup purposes.
		static std::vector< Shader* > shaderList;

	};
}

#endif