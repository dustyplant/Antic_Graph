#include <Antic/Antic_Graph.h>
#include <IL/il.h>
#include <IL/ilu.h>

#include <Antic/Texture.h>
#include <Antic/Shader.h>
#include <Antic/Sprite.h>

#include <glm/gtc/type_ptr.hpp>

#include <math.h>
#include <stack>

//SDL_GLContext agraph::ctx;
//SDL_Window* agraph::window        = nullptr;
GLFWwindow* agraph::window        = nullptr;

glm::mat4 agraph::Model           = glm::mat4( 1.0f );
glm::mat4 agraph::ModelScale 	  = glm::mat4( 1.0f );
glm::mat4 agraph::Projection;
glm::mat4 agraph::ProjectionOrtho;
glm::mat4 agraph::View;

std::stack<glm::mat4> matrixStack;

GLuint screenWidth;
GLuint screenHeight;

bool agraph::initAGraph( std::string title, int width, int height )
{
	//if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	if( glfwInit() == false )
	{
		printf("AGraph Error: Failed to initialize SDL2.\n");
		return cleanup();
	}
	// Set the version of OpenGL to use as version 2.1.
	/*
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	// Require hardware acceleration.
	SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1);

	// Set minimum buffer sizes.
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
	*/

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
	glfwWindowHint( GLFW_SAMPLES, 4 );

	// Create SDL2 Windo
	/*
	window = SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL
	);
	*/
	window = glfwCreateWindow( width, height, title.c_str(), nullptr, nullptr );

	if( window == nullptr )
	{
		printf("AGraph Error: Failed to create SDL2 window.\n");
		return cleanup();
	}

	// Create OpenGL context
	//ctx = SDL_GL_CreateContext( window );
	glfwMakeContextCurrent( window );

	// Initialize GLEW and print any errors.
	GLenum error = glewInit();
	if( error != GLEW_OK )
	{
		printf("%s\n", glewGetErrorString(error));
		return cleanup();
	}


	// Initialize OpenGL

	// Set background color
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	// Enable the third dimension
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glDepthMask( GL_TRUE );

	// Enable transparency
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	// Enable 2D Textures
	glEnable( GL_TEXTURE_2D );

	// Initialize DevIL
	ilInit();
	iluInit();
	ilClearColor( 255, 255, 255, 0 );

	ILenum ilError = ilGetError();
	if( ilError != IL_NO_ERROR )
	{
		printf("%s\n", iluErrorString(ilError));
		return false;
	}

	screenWidth = width;
	screenHeight = height;
	setPerspective();

	// Render the first frame so that the window opens as black.
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	renderDone();
	

	// Set up the camera
	agraph::setCamera();
	agraph::setOrtho( (GLfloat)width, (GLfloat)height );
	agraph::loadIdentityPerspective();

	return true;
}

bool agraph::cleanup()
{
	// Closes the window.
	if( window != nullptr )
	{
		/*
		SDL_GL_DeleteContext( ctx );
		SDL_DestroyWindow( window );
		*/
		glfwDestroyWindow( window );
	}

	agraph::TextureFactory::cleanup();
	agraph::ShaderFactory::cleanup();
	agraph::SpriteSheetFactory::cleanup();

	// Closes SDL2.
	// SDL_Quit();
	glfwTerminate();

	// This is just to clean up code.
	return false;
}

void agraph::renderDone()
{
	//SDL_GL_SwapWindow( window );
	glfwSwapBuffers( window );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void agraph::setCamera( glm::vec3 cameraLocation, glm::vec3 lookingAt, glm::vec3 up )
{
	agraph::View = glm::lookAt( cameraLocation, lookingAt, up );
}

void agraph::setPerspective( GLfloat fov, GLfloat aspectRatio )
{
	agraph::Projection = glm::perspective( (float)(fov * M_PI / 180.f), aspectRatio, 0.1f, 1000000.f );
}

void agraph::setOrtho( GLfloat width, GLfloat height )
{
	agraph::ProjectionOrtho = glm::ortho( 0.f, width, height, 0.f, -1.0f, 1.0f );
}

void agraph::scaleOrtho( GLfloat scaleX, GLfloat scaleY )
{
	int w,h;
	//SDL_GetWindowSize( agraph::window, &w, &h );
	glfwGetWindowSize( agraph::window, &w, &h );
	setOrtho( w * scaleX, h * scaleY );
}

void agraph::scale( GLfloat x, GLfloat y, GLfloat z )
{
	agraph::Model = glm::scale( agraph::Model, glm::vec3( x, y, z ) );
}

void agraph::translate( GLfloat x, GLfloat y, GLfloat z )
{
	agraph::Model = glm::translate( agraph::Model, glm::vec3(x,y,z) );
}

void agraph::rotate( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
	agraph::scale( (GLfloat)getScreenWidth(), (GLfloat)getScreenHeight(), 1.f );
	agraph::Model = glm::rotate( agraph::Model, (float)(angle * M_PI / 180.f), glm::vec3(x,y,z) );
	agraph::scale( 1/(GLfloat)getScreenWidth(), 1/(GLfloat)getScreenHeight(), 1.f );
}

void agraph::rotate2D( GLfloat angle )
{
	agraph::Model = glm::rotate( agraph::Model, (float)(angle * M_PI / 180.f), glm::vec3(0,0,1) );
}

void agraph::loadIdentityPerspective()
{
	agraph::Model      = glm::mat4( 1.f );
	agraph::ModelScale = glm::scale( glm::mat4(1.f), glm::vec3( 1.f/(GLfloat)getScreenWidth(), 1.f/-(GLfloat)getScreenHeight(), 1.f) );
}

void agraph::loadIdentity()
{
	agraph::Model = glm::mat4( 1.f );
}

void agraph::pushMatrix()
{
	matrixStack.push( agraph::Model );
}

void agraph::popMatrix()
{
	if( matrixStack.empty() == false )
	{
		agraph::Model = matrixStack.top();
		matrixStack.pop();
	}
}

GLuint agraph::getScreenWidth()
{
	return screenWidth;
}

GLuint agraph::getScreenHeight()
{
	return screenHeight;
}