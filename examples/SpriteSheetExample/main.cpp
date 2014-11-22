#include <Antic/Antic_Graph.h>
#include <Antic/Texture.h>
#include <Antic/Sprite.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <AGRAPH_Examples_Config.h>

int main( int argc, char *argv[] )
{
	// Intialize the antic graph library.
	if( agraph::initAGraph("Example7", 1024, 768) == false )
		exit( EXIT_FAILURE );

	// Loads the Transforming texture.
	agraph::Texture* tex = agraph::TextureFactory::loadTexture("Transforming", std::string(AGRAPH_RES_PATH) + "exterior-parallaxBG1.png");

	// Loads the sprite sheet of Elisa.
	agraph::SpriteSheet* elisa = agraph::SpriteSheetFactory::loadSS("Elisa.json");

	// The starting and ending sprite locations for the Elisa running animation.
	int startingSprite = 5;
	int endingSprite   = 12;
	
	// Holds the current index of the sprite sheet for the animation of Elisa running.
	int index = startingSprite-1;

	// Angle to rotate the tetxure and the Elisa spritesheet.
	GLfloat angle = 0.f;

	while( glfwWindowShouldClose( agraph::window ) == false )
	{
		glfwPollEvents();

		// Increments the angle to continue the rotation.
		angle += 1;
		if( angle >= 360.f )
			angle = 0.f;

		// Loads the identity.
		agraph::loadIdentity();

		// Translates the texture and the spriteshet to the away from the origin.
		agraph::translate( 250, 250, 0 );

		// Rotates the Model matrix by the given angle.
		agraph::rotate( angle, 1.f, 0.f, 0.f );

		// Renders the Transforming texture.
		tex->render();

		// Loop for the animation.
		index += 1;
		if( index > endingSprite )
			index = startingSprite;


		// Moves the Elisa SpriteSheet object.
		agraph::translate(50.f, 50.f, 0.001f);
		
		// Renders the nth sprite of the sprite sheet.
		elisa->render(index);

		// Renders everything to the window for this frame.
		agraph::renderDone();
	}

	// Cleans up the antic graph library before closing.
	agraph::cleanup();
}