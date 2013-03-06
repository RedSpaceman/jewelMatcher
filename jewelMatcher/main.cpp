#include "SDL.h"
#include "SDL_image.h"

int main( int argc, char* args[] )
{
    //Start SDL
    SDL_Init( SDL_INIT_EVERYTHING );
    
	SDL_Surface* backgroundImage = NULL;
	SDL_Surface* screen = NULL;

	SDL_Event event;

	screen = SDL_SetVideoMode( 755, 600, 32, SDL_SWSURFACE );

	//Load background image
	//backgroundImage = IMG_Load( "C:/Programming/projects/jewelMatcher/images/BackGround.png" );
	backgroundImage = SDL_LoadBMP( "../images/BackGround.bmp" );

	//Apply image to screen
	SDL_BlitSurface( backgroundImage, NULL, screen, NULL );

	//Update screen
	SDL_Flip( screen );

	//Pause
	SDL_Delay( 20000 );

	// Free loaded images
	SDL_FreeSurface( backgroundImage );

    //Quit SDL
    SDL_Quit();
    
    return 0;    
}