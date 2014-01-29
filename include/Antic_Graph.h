#ifndef ANTIC_GRAPH_H
#define ANTIC_GRAPH_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <map>
#include <iostream>

namespace AGraph
{
	static SDL_Window *window = nullptr;
	static SDL_Renderer *renderer = nullptr;
	static SDL_Event event;

	// Maps that store things to be easily accessed later.
	static std::map< std::string, SDL_Texture* > textureMap;
	static std::map< std::string, TTF_Font* > fontMap;
	static std::map< std::string, SDL_Color > colorMap;

	bool initAGraph( std::string title = "", int width = 640, int height = 480 );
	void cleanup();

	void clearTextureMap();
	void clearFontMap();
	void clearColorMap();

	// Loads textures from images.
	SDL_Texture *loadImage( std::string path );
	bool loadImage( std::string name, std::string path );
	SDL_Texture *getTexture( std::string name );

	// Loads fonts.
	bool loadFont( std::string name, std::string path, int fontSize = 12 );
	TTF_Font *loadFont( std::string path, int fontSize = 12 );
	TTF_Font *getFont( std::string name );

	// Loads strings as a texture.
	bool loadWords( std::string name, std::string words, std::string fontName, std::string colorName );
	bool loadWords( std::string name, std::string words, TTF_Font* font, std::string colorName );

	bool loadWords( std::string name, std::string words, std::string fontName, SDL_Color color );
	bool loadWords( std::string name, std::string words, TTF_Font* font, SDL_Color color );

	bool loadColor( std::string name, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 1 );
	bool loadColor( std::string name, SDL_Color newColor );

	// These functions loads and stores colors. You can overwrite colors.
	SDL_Color getColor( std::string name );

	// Render functions.
	void render( std::string name, int x = 0, int y = 0, SDL_Rect *clip = nullptr );
	void render( SDL_Texture *texture, int x = 0, int y = 0, SDL_Rect *clip = nullptr );

	// Call this function when done rendering to display to the screen.
	void renderDone();

	void getWindowSize( int &w, int &h );
}

#endif