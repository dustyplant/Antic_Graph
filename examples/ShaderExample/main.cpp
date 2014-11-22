#include <Antic/Antic_Graph.h>
#include <Antic/Texture.h>
#include <AGRAPH_Examples_Config.h>

int main( int argc, char* argv[] )
{
	int width = 1024;
	int height = 768;

	if( agraph::initAGraph("Shader Example", 1024, 768) == false )
		exit( EXIT_FAILURE );

	// Loads the skyscraper texture using the TextureFactory static class.
	agraph::Texture* tex = agraph::TextureFactory::loadTexture("topdown", std::string(AGRAPH_RES_PATH) + "topdown-sheet3.png");
	
	// Checks if the Tetxure object was loaded correctly.
	if( tex == nullptr )
		exit( EXIT_FAILURE );

	// Loads the shader using the ShaderFactory static class.
	agraph::Shader* programID = agraph::ShaderFactory::loadShaders("TextureShader", "Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");
	if( programID == nullptr )
		exit( EXIT_FAILURE );

	while( glfwWindowShouldClose( agraph::window ) == false )
	{
		glfwPollEvents();

		// Renders the texture using the given shader.
		tex->render( width/2.f, height/2.f, programID);

		// Renders the frame to the screen.
		agraph::renderDone();
	}

	// Cleans up the antic graph engine.
	agraph::cleanup();
	return 0;
}