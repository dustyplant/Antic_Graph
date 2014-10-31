#include <Antic/Antic_Graph.h>
#include <Antic/Sprite.h>

int main( int argc, char* argv[] )
{
	if( agraph::initAGraph("TilesetExmaple", 1024, 768) == false )
		exit( EXIT_FAILURE );

	agraph::SpriteSheetFixed* spriteSheet = dynamic_cast<agraph::SpriteSheetFixed*>(agraph::SpriteSheetFactory::loadSS("res/twilight.json"));
	if( spriteSheet == nullptr )
		exit( EXIT_FAILURE );

	agraph::Texture* tex = agraph::TextureFactory::loadTexture("res/twilight-tiles.png");
	if( tex == nullptr )
		exit( EXIT_FAILURE );

	int tileWidth = spriteSheet->getTileWidth();
	int tileHeight = spriteSheet->getTileHeight();

	SDL_Event event;
	bool quit = false;
	while( quit == false )
	{
		while( SDL_PollEvent( &event ) )
			if( event.type == SDL_QUIT )
				quit = true;

		for( int j = 0; j < tileHeight; ++j )
		{
			for( int i = 0; i < tileWidth; ++i )
			{
				spriteSheet->render( i + tileHeight*j, i*tileWidth, j*tileHeight );
			}
		}
		agraph::renderDone();
	}

	agraph::cleanup();
	exit( EXIT_SUCCESS );
}