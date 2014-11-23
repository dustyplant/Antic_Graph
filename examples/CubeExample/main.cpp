#include <Antic/Antic_Graph.h>
#include <Antic/Shader.h>
#include <glm/glm.hpp>

GLuint vertexArrayID = 0;
GLuint vertexBuffer  = 0;
GLuint colorBuffer   = 0;

/**
 * @brief Draws a Quad or Rect with the given dimensions and location.
 * @details The quad drawn will be rotating along the center point, not any of the corners.
 * 
 * @param x The X position of the top left most corner.
 * @param y The X position of the top left most corner.
 * @param w The width of the quad.
 * @param h The height of the quad.
 * @param angle The angle at which to rotate the quad. Rotates about the center of the quad.
 */
void drawRect( float x, float y, float w, float h, float angle );

/**
 * @brief Draws a cube.
 * @details This is for testing purposes only and will be removed shortly.
 * 
 * @param programID This is the programID of the shader program to be used.
 */
void drawCube( GLuint programID );

// Vertex buffer data for the cube.
const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
     1.0f, 1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
     1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
     1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
     1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
     1.0f,-1.0f, 1.0f
};

// Color buffer data for the cube.
const GLfloat g_color_buffer_data[] = { 
	0.583f,  0.771f,  0.014f,
	0.609f,  0.115f,  0.436f,
	0.327f,  0.483f,  0.844f,
	0.822f,  0.569f,  0.201f,
	0.435f,  0.602f,  0.223f,
	0.310f,  0.747f,  0.185f,
	0.597f,  0.770f,  0.761f,
	0.559f,  0.436f,  0.730f,
	0.359f,  0.583f,  0.152f,
	0.483f,  0.596f,  0.789f,
	0.559f,  0.861f,  0.639f,
	0.195f,  0.548f,  0.859f,
	0.014f,  0.184f,  0.576f,
	0.771f,  0.328f,  0.970f,
	0.406f,  0.615f,  0.116f,
	0.676f,  0.977f,  0.133f,
	0.971f,  0.572f,  0.833f,
	0.140f,  0.616f,  0.489f,
	0.997f,  0.513f,  0.064f,
	0.945f,  0.719f,  0.592f,
	0.543f,  0.021f,  0.978f,
	0.279f,  0.317f,  0.505f,
	0.167f,  0.620f,  0.077f,
	0.347f,  0.857f,  0.137f,
	0.055f,  0.953f,  0.042f,
	0.714f,  0.505f,  0.345f,
	0.783f,  0.290f,  0.734f,
	0.722f,  0.645f,  0.174f,
	0.302f,  0.455f,  0.848f,
	0.225f,  0.587f,  0.040f,
	0.517f,  0.713f,  0.338f,
	0.053f,  0.959f,  0.120f,
	0.393f,  0.621f,  0.362f,
	0.673f,  0.211f,  0.457f,
	0.820f,  0.883f,  0.371f,
	0.982f,  0.099f,  0.879f
};


int main( int argc, char* argv[] ) {
	if( agraph::initAGraph("Cube Example") == false )
		exit( EXIT_FAILURE );

	// Loads the default shaders supplied by the engine.
	// GLuint programID = agraph::loadShadersString(agraph::vertexShaderString, agraph::fragmentShaderString);

	// Loads the custom shaders using the ShaderFactory static class.
	agraph::Shader* shader = agraph::ShaderFactory::loadShaders("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");

	// Generates the vertex array object;
	GLuint vertexArrayID = 0;
	glGenVertexArrays( 1, &vertexArrayID );
	glBindVertexArray( vertexArrayID );

	// Generates and binds the vertex buffer, then loads it with the vertex buffer data.
	// This is basic OpenGL not wrapped by the Antic_Graph library.
	glGenBuffers( 1, &vertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW );

	// Generates and binds the color buffer, then loads it with the color buffer data.
	// This is basic OpenGL not wrapped by the Antic_Graph library.
	glGenBuffers( 1, &colorBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, colorBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW );

	// Sets the camera using the antic graph method.
	// The camera is at location (4,3,3) in the world coordinates and is looking at (0,0,0) in world coordinates.
	agraph::setCamera( glm::vec3(4,3,3), glm::vec3(0,0,0) );

	while( glfwWindowShouldClose( agraph::window ) == false )
	{
		glfwPollEvents();

		// Binds the shader.
		shader->bind();

		// Draws a rectangle to the screen.
		//agraph::drawRect( 100.0f, 100.0f, 50.0f, 50.0f, 150.f );

		// Draws the cube.
		drawCube( shader->getProgramID() );

		// Renders the frame to the window.
		agraph::renderDone();
	}

	// Deallocates the vertex array object.
	if( vertexArrayID != 0 )
		glDeleteVertexArrays( 1, &vertexArrayID );

	// Deallocates the vertex buffer object.
	if( vertexBuffer != 0 )
		glDeleteBuffers( 1, &vertexBuffer );

	// Deallocates the color buffer object.
	if( colorBuffer != 0 )
		glDeleteBuffers( 1, &colorBuffer );

	// Cleans up the antic graph library.
	agraph::cleanup();
	return 0;
}

void drawCube( GLuint programID )
{
	// Use the given shader.
	glUseProgram( programID );

	// Multiply matrices to get the MVC (Model, View, Projection matrix).
	glm::mat4 MVP = agraph::Projection * agraph::View * glm::mat4(1.f);

	// Get the location of the MVP variable in the shader to later pass the MVP matrix data to it.
	GLuint MatrixID = glGetUniformLocation( programID, "MVP" );

	// Passes the MVP matrix to the shader.
	glUniformMatrix4fv( MatrixID, 1, GL_FALSE, &MVP[0][0] );

	// Get the vertexPosition and vertexColor variable locations from the shader to later be loaded with data.
	GLuint vpLocation = glGetAttribLocation( programID, "vertexPosition" );
	GLuint vcLocation = glGetAttribLocation( programID, "vertexColor" );

	/*
	// You can also bind the shader variables like this.
	glBindAttribLocation( programID, 0, "vertexPosition" );
	glBindAttribLocation( programID, 1, "vertexColor");
	*/
	// Load the vertexPosition variable with the vertex buffer data.
	glEnableVertexAttribArray( vpLocation );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glVertexAttribPointer( vpLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	// Load the vertexColor variable with the color buffer data.
	glEnableVertexAttribArray( vcLocation );
	glBindBuffer( GL_ARRAY_BUFFER, colorBuffer );
	glVertexAttribPointer( vcLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	// This line renders the data passed to the shader.
	glDrawArrays( GL_TRIANGLES, 0, 12*3 );

	// Closes connections to the variables in the shader.
	glDisableVertexAttribArray( vpLocation );
	glDisableVertexAttribArray( vcLocation );
}

void drawRect( float x, float y, float w, float h, float angle )
{
	float w2 = w/2.0;
	float h2 = h/2.0;

	// Save the OpenGL state by pushing the current state onto the stack.
	glPushMatrix();

	// Translates and rotates the OpenGL matrix for drawing to the proper place.
	glTranslatef( x + w2, y + h2, 0.0f );
	glRotatef( angle, 0, 0, 1 );

	// The actual drawing.
	glBegin( GL_TRIANGLE_FAN );
		glVertex2f( -w2, -h2 );
		glVertex2f(  w2, -h2 );
		glVertex2f(  w2,  h2 );
		glVertex2f( -w2,  h2 );
	glEnd();

	// Reapplies the previous OpenGL state.
	glPopMatrix();
}