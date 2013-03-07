#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>

#include "Jewel.h"

SDL_Surface* backgroundImage = NULL;
SDL_Surface* screen = NULL;

SDL_Surface* redJewel = NULL;
SDL_Surface* blueJewel = NULL;
SDL_Surface* yellowJewel = NULL;
SDL_Surface* greenJewel = NULL;
SDL_Surface* purpleJewel = NULL;

SDL_Event event;

int SCREEN_WIDTH = 755;
int SCREEN_HEIGHT = 600; 
int SCREEN_BPP = 32;

// Load images and convert them to the right BPP
SDL_Surface *load_image( std::string filename )
{
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;

	loadedImage = IMG_Load( filename.c_str() );

	if (loadedImage != NULL )
	{
		optimizedImage = SDL_DisplayFormat( loadedImage );
		SDL_FreeSurface( loadedImage );

		if ( optimizedImage != NULL )
		{
			// Get color values
			Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0, 0 );
			// Map a color to transparency
			SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
		}
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

void drawGrid()
{
	// Centre of jewel grid coordinates, manually determined
	int xCentre = 500;
	int yCentre = 275;
	// Distance between each jewel render location
	int step = 42;
	// Number of jewels on one 
	int gridWidth = 8;

	// Based on arbitary grid width provided, determine grid starting coordinates
	int xStart = (int)(xCentre-(step*(gridWidth/2)));
	int yStart = (int)(yCentre-(step*(gridWidth/2)));
	// Offset grid drawing if odd-numbered grid width
	if ( gridWidth%2 != 0 )
	{
		xStart -= step/2;
		yStart -= step/2;
	}

	// Draw grid of random jewels
	for (int i = 0; i < gridWidth; i++)
	{
		for (int j = 0; j < gridWidth; j++ )
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
}

bool init() 
{
	//Start SDL
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		return false;
	}

	// Set up screen
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
	if ( screen == NULL)
	{
		return false;
	}

	// Set window caption
	SDL_WM_SetCaption( "Jewel Matcher event", NULL );

	return true;
}

bool load_files()
{
	//Load background image
	backgroundImage = load_image( "../images/BackGround.jpg" );
	if ( backgroundImage == NULL ) return false;
	//load red
	redJewel = load_image( "../images/Red.png" );
	if ( redJewel == NULL ) return false;
	//load blue
	blueJewel = load_image( "../images/Blue.png" );
	if ( blueJewel == NULL ) return false;
	//load green
	greenJewel = load_image( "../images/Green.png" );
	if ( greenJewel == NULL ) return false;
	//load yellow
	yellowJewel = load_image( "../images/Yellow.png" );
	if ( yellowJewel == NULL ) return false;
	//load purple
	purpleJewel = load_image( "../images/Purple.png" );
	if ( purpleJewel == NULL ) return false;	

	return true;
}

void clean_up()
{
	// Free loaded images
	SDL_FreeSurface( backgroundImage );

    //Quit SDL
    SDL_Quit();
}

int main( int argc, char* args[] )
{
	bool quit = false;

	if ( init() == false ){
		return 1;
	}

	if ( load_files() == false )
	{
		return 1;
	}

	//Apply image to screen
	apply_surface( 0, 0, backgroundImage, screen );
	drawGrid();

	//Update screen
	if ( SDL_Flip( screen ) == -1 )
	{
		return 1;
	}

	//Pause
	while( quit == false )
	{
		while( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_QUIT )
			{
				//Exit
				quit = true;
			}
		}
	}

	// Free-ip surfaces and quit SDL
	clean_up();
    
    return 0;    
}

