#include <Antic/Antic_Graph.h>
#include <Antic/Texture.h>
#include <Antic/Sprite.h>
#include <Antic/Shader.h>

int main( int argc, char* argv[] )
{
	if( agraph::initAGraph("Loading SpriteSheets", 1024, 768) == false )
		exit( EXIT_FAILURE );

	agraph::SpriteSheet* spriteSheet = agraph::SpriteSheetFactory::loadSS("TestSpriteSheet.json");
	if( spriteSheet == nullptr )
	{
		printf("Error loading SpriteSheet from JSON file.\n");
		delete spriteSheet;
		exit( EXIT_FAILURE );	
	}

	int i = 0;
	while( glfwWindowShouldClose( agraph::window ) == false )
	{
		glfwPollEvents();

		++i;
		if( i >= 7 )
			i = 0;
		spriteSheet->render( i );
		agraph::renderDone();
	}

	agraph::cleanup();
}