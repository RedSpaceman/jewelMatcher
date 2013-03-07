#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>

#include "Jewel.h"

// Load images and convert them to the right BPP
SDL_Surface *load_image( std::string filename )
{
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;

	loadedImage = SDL_LoadBMP( filename.c_str() );

	if (loadedImage != NULL )
	{
		optimizedImage = SDL_DisplayFormat( loadedImage );
		SDL_FreeSurface( loadedImage );
	}

	return optimizedImage;
}

// Apply surface using Blitting
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
	// SDL_BlitSurface() only accepts offsets inside of an SDL_Rect
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;

	SDL_BlitSurface( source, NULL, destination, &offset );
}

int main( int argc, char* args[] )
{
    //Start SDL
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) return 1;

	SDL_Surface* backgroundImage = NULL;
	SDL_Surface* screen = NULL;

	SDL_Surface* redJewel = NULL;
	SDL_Surface* blueJewel = NULL;
	SDL_Surface* yellowJewel = NULL;
	SDL_Surface* greenJewel = NULL;
	SDL_Surface* purpleJewel = NULL;

	SDL_Event event;

	// Set up screen
	int SCREEN_WIDTH = 755;
	int SCREEN_HEIGHT = 600; 
	int SCREEN_BPP = 32;
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
	if ( screen == NULL)
	{
		return 1;
	}
	SDL_WM_SetCaption( "Jewel Matcher", NULL );

	//Load background image
	//backgroundImage = IMG_Load( "C:/Programming/projects/jewelMatcher/images/BackGround.png" );
	backgroundImage = load_image( "../images/BackGround.bmp" );
	redJewel = load_image( "../images/Red.bmp" );
	blueJewel = load_image( "../images/Blue.bmp" );
	greenJewel = load_image( "../images/Green.bmp" );
	yellowJewel = load_image( "../images/Yellow.bmp" );
	purpleJewel = load_image( "../images/Purple.bmp" );

	//Apply image to screen
	apply_surface( 0, 0, backgroundImage, screen );
	//apply_surface( 340, 120, redJewel, screen );

	int xStart = 340;
	int yStart = 116;
	int step = 40;

	// Draw grid of random jewels
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++ )
		{
			Jewel* jewel = new Jewel(xStart+(step*i), yStart+(step*j));
			
			char jewelType = jewel->getJewelType();

			switch (jewelType)
			{
				case 'r': apply_surface( jewel->getX(), jewel->getY(), redJewel, screen);
				break;
				case 'g': apply_surface( jewel->getX(), jewel->getY(), greenJewel, screen);
				break;
				case 'y': apply_surface( jewel->getX(), jewel->getY(), yellowJewel, screen);
				break;
				case 'p': apply_surface( jewel->getX(), jewel->getY(), purpleJewel, screen);
				break;
				case 'b': apply_surface( jewel->getX(), jewel->getY(), blueJewel, screen);
			}
		}
	}

	//Update screen
	if ( SDL_Flip( screen ) == -1 )
	{
		return 1;
	}

	//Pause
	SDL_Delay( 10000 );

	// Free loaded images
	SDL_FreeSurface( backgroundImage );

    //Quit SDL
    SDL_Quit();
    
    return 0;    
}

