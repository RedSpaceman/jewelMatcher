#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "Jewel.h"
#include "Button.h"
#include "Grid.h"

SDL_Surface* backgroundImage = NULL;
SDL_Surface* screen = NULL;

SDL_Surface* redJewel = NULL;
SDL_Surface* blueJewel = NULL;
SDL_Surface* yellowJewel = NULL;
SDL_Surface* greenJewel = NULL;
SDL_Surface* purpleJewel = NULL;

SDL_Surface* socketDefault = NULL;
SDL_Surface* socketHover = NULL;
SDL_Surface* socketSelected = NULL;

SDL_Event event;

// Font
TTF_Font *font = NULL;
SDL_Color textColor = { 10, 10, 10 };
SDL_Surface* message = NULL;

int SCREEN_WIDTH = 755;
int SCREEN_HEIGHT = 600; 
int SCREEN_BPP = 32;

// Number of jewels on one side of the grid
int GRID_SIZE = 8;
int CELL_W = 42;
int CELL_H = 42;
// Centre of jewel grid coordinates, manually determined
int GRID_CENTRE_X = 500;
int GRID_CENTRE_Y = 275;

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
			// Set black as colorKey
			Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0, 0 );
			// Map a colorkey to transparency
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

void drawGridManually()
{
	// Based on arbitary grid width provided, determine grid starting coordinates
	int xStart = (int)(GRID_CENTRE_X-(CELL_W*(GRID_SIZE/2)));
	int yStart = (int)(GRID_CENTRE_Y-(CELL_W*(GRID_SIZE/2)));
	// Offset grid drawing if odd-numbered grid width
	if ( GRID_SIZE%2 != 0 )
	{
		xStart -= CELL_W/2;
		yStart -= CELL_W/2;
	}

	// Draw grid of random jewels
	for (int i = 0; i < GRID_SIZE; i++)
	{
		for (int j = 0; j < GRID_SIZE; j++ )
		{
			// Generate and draw socket
			Button* button = new Button(xStart+(CELL_W*i), yStart+(CELL_W*j), CELL_W, CELL_W);

			// Generate and draw jewel
			Jewel* jewel = new Jewel(xStart+(CELL_W*i), yStart+(CELL_W*j));
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

// Loop over sockets, drawing their images and the jewels they contain
void drawGrid(Grid* gameGrid)
{
	//std::cout<<"sockets size: "<< gameGrid->sockets.size() << std::endl;
	std::vector<Socket*>::iterator socketsEnd = gameGrid->getSocketsEnd();
	for( std::vector<Socket*>::iterator it = gameGrid->getSocketsBeginning(); it != socketsEnd; it++)
	{
		// Set socket surface alphas
		int alpha = (int)( (SDL_ALPHA_OPAQUE - SDL_ALPHA_TRANSPARENT) * 0.3 );
		SDL_SetAlpha( socketSelected, SDL_SRCALPHA, alpha );
		// For each Socket in sockets, draw background
		Socket* nextSocket = *it;
		int socketXStart = nextSocket->getX() - (nextSocket->getWidth()/2);
		int socketYStart = nextSocket->getY() - (nextSocket->getHeight()/2);
		apply_surface( socketXStart, socketYStart, socketSelected, screen);

		// Draw jewel
		char jewelType = nextSocket->getCurrentJewelType();
		int jewelX = nextSocket->getCurrentJewel()->getX();
		int jewelY = nextSocket->getCurrentJewel()->getY();
		// Use surface dims to centre the jewel images
		switch (jewelType)
		{
			case 'r': apply_surface( jewelX-(redJewel->w/2), jewelY-(redJewel->h/2), redJewel, screen);
			break;
			case 'g': apply_surface( jewelX-(greenJewel->w/2), jewelY-(greenJewel->h/2), greenJewel, screen);
			break;
			case 'y': apply_surface( jewelX-(yellowJewel->w/2), jewelY-(yellowJewel->h/2), yellowJewel, screen);
			break;
			case 'p': apply_surface( jewelX-(purpleJewel->w/2), jewelY-(purpleJewel->h/2), purpleJewel, screen);
			break;
			case 'b': apply_surface( jewelX-(blueJewel->w/2), jewelY-(blueJewel->h/2), blueJewel, screen);
			break;
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

	// Initialise truetype font
	if ( TTF_Init() == -1 )
	{
		return false;
	}

	// Set window caption
	SDL_WM_SetCaption( "Jewel Matcher", NULL );

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

	//load socket images
	socketDefault = load_image( "../images/socketDefault.png" );
	if ( socketDefault == NULL ) return false;
	socketHover = load_image( "../images/socketHover.png" );
	if ( socketHover == NULL ) return false;
	socketSelected = load_image( "../images/socketSelected.png" );
	if ( socketSelected == NULL ) return false;

	// Open font
	font = TTF_OpenFont( "lazy.ttf", 28 );
	if ( font == NULL )
	{
		return false;
	}

	return true;
}

void clean_up()
{
	// Free loaded images
	SDL_FreeSurface( backgroundImage );
	SDL_FreeSurface( redJewel );
	SDL_FreeSurface( greenJewel );
	SDL_FreeSurface( purpleJewel );
	SDL_FreeSurface( blueJewel );
	SDL_FreeSurface( yellowJewel );
	SDL_FreeSurface( socketDefault );
	SDL_FreeSurface( socketHover );
	SDL_FreeSurface( socketSelected );

	// Clean up font
	SDL_FreeSurface( message );
	TTF_CloseFont( font );
	TTF_Quit();

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

	message = TTF_RenderText_Solid( font, "Jewel Matcher", textColor );
	if ( message == NULL )
	{
		return 1;
	}

	SDL_Surface* upMessage = TTF_RenderText_Solid( font, "Up pressed", textColor );
	SDL_Surface* downMessage = TTF_RenderText_Solid( font, "Down pressed", textColor );

	//Apply image to screen
	apply_surface( 0, 0, backgroundImage, screen );

	// Construct Grid of Sockets and Jewels
	// Based on arbitary grid width provided, determine grid starting coordinates
	Grid* gameGrid = new Grid(GRID_SIZE, GRID_CENTRE_X, GRID_CENTRE_Y, CELL_W, CELL_H);

	//drawGridManually();
		
	//Pause
	while( quit == false )
	{
		// Event handling
		while( SDL_PollEvent( &event ) )
		{
			// When exiting the program
			if( event.type == SDL_QUIT )
			{
				//Exit
				quit = true;
			}

			if ( event.type == SDL_KEYDOWN )
			{
				switch( event.key.keysym.sym )
				{
					case SDLK_UP: message = upMessage; break;
					case SDLK_DOWN: message = downMessage; break;
				}
			}
		}

		// Refresh message
		if ( message != NULL )
		{
			apply_surface( 0, 0, backgroundImage, screen );
			//drawGridManually();
			drawGrid( gameGrid );
			apply_surface( 400, 55, message, screen);
			message = NULL;
		}

		//Update screen
		if ( SDL_Flip( screen ) == -1 )
		{
			return 1;
		}
	}

	// Free-ip surfaces and quit SDL
	clean_up();
    
    return 0;    
}

