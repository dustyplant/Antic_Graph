#include <Antic/Antic_Graph.h>
#include <Antic/Texture.h>
#include <Antic/Sprite.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

int main( int argc, char *argv[] )
{
	// Intialize the antic graph library.
	if( agraph::initAGraph("Example7", 1024, 768) == false )
		exit( EXIT_FAILURE );

	// Loads the Transforming texture. This is the skyscraper picture.
	agraph::Texture* tex = agraph::TextureFactory::loadTexture("Transforming", "../../../res/exterior-parallaxBG1.png");

	// Loads the sprite sheet of Elisa.
	agraph::SpriteSheet* elisa = agraph::SpriteSheetFactory::loadSS("Elisa.json");

	// The starting and ending sprite locations for the Elisa running animation.
	int startingSprite = 5;
	int endingSprite   = 12;
	
	// Holds the current index of the sprite sheet for the animation of Elisa running.
	int index = startingSprite-1;

	bool quit = false;
	SDL_Event event;
	while( quit == false )
	{
		while( SDL_PollEvent( &event ) )
			if( event.type == SDL_QUIT )
				quit = true;


		// Renders the skyscraper texture.
		tex->render();

		// Loop for the animation.
		index += 1;
		if( index > endingSprite )
			index = startingSprite;

		// Renders the nth sprite of the sprite sheet.
		elisa->render(index);

		// Renders everything to the window for this frame.
		agraph::renderDone();
	}

	// Cleans up the antic graph library before closing.
	agraph::cleanup();
}