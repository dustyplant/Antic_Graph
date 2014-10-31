#include <Antic/Antic_Graph.h>
#include <Antic/Sprite.h>

int main( int argc, char* argv[] )
{
	if( agraph::initAGraph("TilesetExmaple", 1024, 768) == false )
		exit( EXIT_FAILURE );

	agraph::SpriteSheet* spriteSheet = agraph::SpriteSheetFactory::loadSS("res/twilight.json");
	if( spriteSheet == nullptr )
		exit( EXIT_FAILURE );

	agraph::Texture* tex = agraph::TextureFactory::loadTexture("res/twilight-tiles.png");
	if( tex == nullptr )
		exit( EXIT_FAILURE );

	SDL_Event event;
	bool quit = false;
	while( quit == false )
	{
		while( SDL_PollEvent( &event ) )
			if( event.type == SDL_QUIT )
				quit = true;
		
		for( int j = 0; j < 16; ++j )
		{
			for( int i = 0; i < 16; ++i )
			{
				spriteSheet->render( i + 16*j );
				agraph::translate( 16 + 1, 0 );
			}
			agraph::translate( -256 - 16, 0 );
			agraph::translate( 0, 16 + 1 );
		}
		agraph::translate( 0, -256 - 16 );
		agraph::renderDone();
	}

	agraph::cleanup();
	exit( EXIT_SUCCESS );
}