#ifndef ANTIC_GRAPH_H
#define ANTIC_GRAPH_H

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>

namespace agraph {
	/// Pointer to the SDL2 Window. This is what everything is being drawn to.
	extern SDL_Window *window;

	/// OpenGL Context object. It is created from the SDL2 window object.
	extern SDL_GLContext ctx;

	/// Projection Matrix
	extern glm::mat4 Projection;
	extern glm::mat4 ProjectionOrtho;

	/// View Matrix
	extern glm::mat4 View;

	/// Model Matrix
	extern glm::mat4 Model;


	/**
	 * @brief Initializes Antic_Graph, SDL2 and OpenGL. Creates a window with given properties.
	 * 
	 * @param title The title of the window.
	 * @param width The width of the window.
	 * @param height The height of the window.
	 * 
	 * @return Whether or not the initialization was successful.
	 */
	bool initAGraph( std::string title="", int width=640, int height=480 );

	/**
	 * @brief Cleans up and closes everything that Antic_Graph used or initialized.
	 * @details All pointers cleared, including those without a mapped name. Closes SDL2.
	 * @return False for cleaner source code reasons.
	 */
	bool cleanup();

	/**
	 * @brief Call this when you are done rendering everything for the given frame.
	 * @details Draws everything rendered to the window and then clears the buffers so you can render the next frame.
	 */
	void renderDone();


	/**
	 * @brief Translates the matrix to a certain position.
	 * @details Same as glTranslatef.
	 * 
	 * @param x The x offset to translate.
	 * @param y The y offset to translate. Note +y is down and -y is up.
	 * @param z The z offset to translate.
	 */
	void translate( GLfloat x, GLfloat y, GLfloat z = 0.0f );


	/**
	 * @brief Rotates the matrix based on the axis specified.
	 * 
	 * @param angle The angle in degrees in which to rotate.
	 * @param x Whether or not to rotate by the x axis. Pass 1.f if you want to rotate by this axis.
	 * @param y Whether or not to rotate by the y axis. Pass 1.f if you want to rotate by this axis.
	 * @param z Whether or not to rotate by the z axis. Pass 1.f if you want to rotate by this axis.
	 */
	void rotate( GLfloat angle, GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z = 1.0f );

	/**
	 * @brief Rotates in 2D space.
	 * @details Rotates along the z axis.
	 * 
	 * @param angle Angle to rotate by in degrees.
	 */
	void rotate2D( GLfloat angle );

	/**
	 * @brief Scale the model matrix.
	 * 
	 * @param w The amount to scale the x axis by.
	 * @param h The amount to scale the y axis by.
	 * @param z The amount to scale the z axis by.
	 */
	void scale( GLfloat x, GLfloat y, GLfloat z );

	/**
	 * @brief Scale the orthographic matrix.
	 * 
	 * @param scaleX How much to scale the orthographic matrix by. (i.e. Pass 2 to multiply the matrix by 2).
	 * @param scaleY How much to scale teh orthographic matrix by.
	 */
	void scaleOrtho( GLfloat scaleX, GLfloat scaleY );


	/**
	 * @brief Pushes the Model matrix onto an internal stack.
	 * @details Use this function to save the Model matrix. You can then modify it and return to the previous Model by popping it off the stack.
	 */
	void pushMatrix();

	/**
	 * @brief Pops the last Model matrix off the stack. If the stack is empty, it will return the identity matrix.
	 */
	void popMatrix();

	/**
	 * @brief Loads the identity matrix without scaling. Use this for orthogonal projection matrices.
	 */
	void loadIdentity();

	/**
	 * @brief Returns the identity matrix but scaled to for the window size.
	 */
	void loadIdentityPerspective();

	/**
	 * @brief Sets the camera. The default values set the camera to have the origin in the top left corner.
	 * 
	 * @param cameraLocation The location of the camera in space.
	 * @param lookingAt The point that the camera is looking towards.
	 * @param up The vector describing which way is up. For most cases just use the default.
	 */
	void setCamera( glm::vec3 cameraLocation = glm::vec3(0.5,-0.5,0.865), glm::vec3 lookingAt = glm::vec3(0.5,-0.5,0), glm::vec3 up=glm::vec3(0,1,0) );

	// TODO: This
	// void translateCameraFlat( GLfloat x, GLfloat y, GLfloat z = 0.f );

	/**
	 * @brief Sets the width and height of the orthographic matrix.
	 * 
	 * @param width The width to set the orthographic matrix to.
	 * @param height The height to set the orthographic matrix to.
	 */
	void setOrtho( GLfloat width, GLfloat height );

	/**
	 * @brief Sets the perspective Projection matrix. This is separate from the orthogonal Projection matrix.
	 * @param fov The angle in degrees to set the field of view to.
	 */
	void setPerspective( GLfloat fov = 60.f, GLfloat aspectRatio = 1.f );

	/**
	 * @brief Returns the width of the window.
	 * @return The width of the window without any scaling.
	 */
	GLuint getScreenWidth();

	/**
	 * @brief Returns the height of the window.
	 * @return The height of the window without any scaling.
	 */
	GLuint getScreenHeight();


	/// The default vertex shader.
	static const std:: string vertexShaderString = "#version 120\n"

	"attribute vec3 vertexPosition;\n"
	"attribute vec3 vertexColor\n;"

	"varying vec3 fragmentColor;\n"

	"uniform mat4 MVP;\n"

	"void main() {\n"
		"gl_Position = MVP * vec4( vertexPosition, 1 );\n"
	
		"fragmentColor = vertexColor;\n"
	"}\n";


	/// The default fragment shader.
	static const std::string fragmentShaderString = "#version 120\n"

	"varying vec3 fragmentColor;\n"

	"void main() {\n"
		"gl_FragColor = vec4(fragmentColor, 1);\n"
	"}\n";


	/// Simply a struct that holds four floats that can be used to store the x, y, width, and height of a rectangle.
	struct Rect
	{
		GLfloat x;
		GLfloat y;
		GLfloat w;
		GLfloat h;
	};
}

#endif