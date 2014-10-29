#include <Antic/Antic_Graph.h>
#include <Antic/Texture.h>
#include <Antic/Shader.h>
#include <Antic/Sprite.h>

int main( int argc, char* argv[] )
{
	if( agraph::initAGraph("Sprite Example", 800, 600) == false )
		exit( EXIT_FAILURE );

	// Loads the topdown texture using the TextureFactory static class.
	agraph::Texture* tex = agraph::TextureFactory::loadTexture("topdown", "../../../res/topdown-sheet3.png");
	
	// Checks to make sure that the texture loaded.
	if( tex == nullptr )
		exit( EXIT_FAILURE );

	// Define a sprite manually. This must be deallocated manually.
	agraph::Sprite* sprite = new agraph::Sprite(tex, 42, 22, 54, 95);

	agraph::Rect clipData = sprite->getClipData();

	SDL_Event event;
	bool quit = false;
	while( quit == false )
	{
		while( SDL_PollEvent( &event ) )
			if( event.type == SDL_QUIT )
				quit = true;

		// Renders the sprite at the given x,y location.
		sprite->render( agraph::getScreenWidth() - clipData.w, agraph::getScreenHeight() - clipData.h );
		sprite->render();

		// Renders the frame to the screen.
		agraph::renderDone();
	}

	// Deallocates the sprite manually.
	delete sprite;

	// Cleans up the antic graph library.
	agraph::cleanup();
}