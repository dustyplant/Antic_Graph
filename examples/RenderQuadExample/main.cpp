#include <Antic/Antic_Graph.h>
#include <Antic/Shader.h>

// Vertex buffer Data for the texture.
const GLfloat vertexData[] = {
	 0.f,  0.f, 0.0f,
	 0.f, 50.f, 0.0f,
	50.f, 50.f, 0.0f,
	50.f,  0.f, 0.0f
};

// Color buffer data for the texture.
const GLfloat colorData[] = {
	1.f, 0.25f, 1.f,
	1.f, 0.25f, 1.f,
	1.f, 0.25f, 1.f,
	1.f, 0.25f, 1.f
};

int main( int argc, char* argv[] )
{
	if( agraph::initAGraph("Render Quad Example", 640, 480) == false )
		exit( EXIT_FAILURE );

	// Loads a shader using the ShaderFactory static class. Loads the shader with a shader string, not a file.
	GLuint programID = agraph::ShaderFactory::loadShadersString("Default", agraph::vertexShaderString, agraph::fragmentShaderString)->getProgramID();
	
	// Checks if the shader loaded correctly.
	if( programID == 0 )
		exit( EXIT_FAILURE );

	GLuint vertexArrayID = 0;
	glGenVertexArrays( 1, &vertexArrayID );
	glBindVertexArray( vertexArrayID );

	// Generates and binds the vertex buffer, then loads it with the vertex buffer data.
	// This is basic OpenGL not wrapped by the Antic_Graph library.
	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// Generates and binds the color buffer, then loads it with the color buffer data.
	// This is basic OpenGL not wrapped by the Antic_Graph library.
	GLuint colorBufferID;
	glGenBuffers(1, &colorBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

	// Loads the identity for the orthographic projection.
	agraph::loadIdentity();

	SDL_Event event;
	bool quit = false;
	while( quit == false )
	{
		while( SDL_PollEvent( &event ) )
			if( event.type == SDL_QUIT )
				quit = true;

		// Loads the shader.
		glUseProgram( programID );

		// Multiply matrices to get the MVC (Model, View, Projection matrix).
		glm::mat4 MVP = agraph::ProjectionOrtho * agraph::View * glm::translate(agraph::Model, glm::vec3(10.f, 10.f, 0.0f));
		
		// Get the location of the MVP variable in the shader to later pass the MVP matrix data to it.
		GLuint MatrixID = glGetUniformLocation( programID, "MVP" );

		// Passes the MVP matrix to the shader.
		glUniformMatrix4fv( MatrixID, 1, GL_FALSE, &MVP[0][0] );

		// Get the vertexPosition and vertexColor variable locations from the shader to later be loaded with data.
		GLuint vertexPosition = glGetAttribLocation( programID, "vertexPosition" );
		GLuint vertexColor = glGetAttribLocation( programID, "vertexColor" );

		// Load the vertexPosition variable with the vertex buffer data.
		glEnableVertexAttribArray( vertexPosition );
		glBindBuffer( GL_ARRAY_BUFFER, vertexBufferID );
		glVertexAttribPointer( vertexPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

		// Load the vertexColor variable with the color buffer data.
		glEnableVertexAttribArray( vertexColor );
		glBindBuffer( GL_ARRAY_BUFFER, colorBufferID );
		glVertexAttribPointer( vertexColor, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

		// This line renders the data passed to the shader.
		glDrawArrays( GL_TRIANGLE_FAN, 0, 3*4 );

		// Closes connections to the variables in the shader.
		glDisableVertexAttribArray( vertexPosition );
		glDisableVertexAttribArray( vertexColor );

		// Renders everything to the screen for this frame.
		agraph::renderDone();
	}

	if( vertexArrayID != 0 )
		glDeleteVertexArrays( 1, &vertexArrayID );

	// Frees the vertex buffer object.
	if( vertexBufferID != 0 )
		glDeleteBuffers( 1, &vertexBufferID );

	// Frees the color buffer object.
	if( colorBufferID != 0 )
		glDeleteBuffers( 1, &colorBufferID );

	// Cleans up the antic graph library.
	agraph::cleanup();
}