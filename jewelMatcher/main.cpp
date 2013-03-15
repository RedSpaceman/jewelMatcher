#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "time.h"
#include <iostream>
#include <vector>
#include <sstream>

#include "Jewel.h"
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
std::vector<Socket*> selectedSockets;

// Font
TTF_Font *font = NULL;
SDL_Color textColor = { 10, 10, 10 };
SDL_Surface* message = NULL;

int SCREEN_WIDTH = 755;
int SCREEN_HEIGHT = 600; 
int SCREEN_BPP = 32;

// Length of time for a game
int totalGameLength = 60000;

// Number of jewels on one side of the grid
int GRID_SIZE = 8;
int CELL_W = 42;
int CELL_H = 42;
// Centre of jewel grid coordinates, manually determined
int GRID_CENTRE_X = 500;
int GRID_CENTRE_Y = 275;

// Track mouse coordinates
int mouseX = 0;
int mouseY = 0;

// Game score counts points player has earned during current game
int gameScore = 0;

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

// Loop over sockets, drawing their images and the jewels they contain
void drawGrid( Grid* const &gameGrid )
{
	std::vector<Socket*>::iterator socketsBegin = gameGrid->getSocketsBeginning();
	std::vector<Socket*>::iterator socketsEnd = gameGrid->getSocketsEnd();
	for( std::vector<Socket*>::iterator it = socketsBegin; it != socketsEnd; it++ )
	{
		// Set socket surface alphas
		int alpha = (int)( (SDL_ALPHA_OPAQUE - SDL_ALPHA_TRANSPARENT) * 0.5 );
		SDL_SetAlpha( socketSelected, SDL_SRCALPHA, alpha );
		alpha = (int)( (SDL_ALPHA_OPAQUE - SDL_ALPHA_TRANSPARENT) * 0.4);
		SDL_SetAlpha( socketDefault, SDL_SRCALPHA, alpha );
		// For each Socket in sockets, draw background
		Socket* nextSocket = *it;
		SDL_Rect socketBound = nextSocket->getSocketBound();
		// Draw socket image dependent on whether socket is selected
		if( ( selectedSockets.size() > 0 && ( nextSocket == selectedSockets.at(0) ) ) 
			|| ( selectedSockets.size() > 1 && ( nextSocket == selectedSockets.at(1) ) ) )
		{
			apply_surface( socketBound.x, socketBound.y, socketSelected, screen);
		}
		else
		{
			apply_surface( socketBound.x, socketBound.y, socketDefault, screen);
		}

		// Draw jewel
		Jewel* currentJewel = nextSocket->getCurrentJewel();
		// Can only draw jewel if one exists in socket
		if( currentJewel != NULL )
		{
			char jewelType = nextSocket->getCurrentJewelType();
			SDL_Rect jewelBound = currentJewel->getJewelBound();
			SDL_Rect jewelDest = currentJewel->getDestination();
		
			int jewelDrawBoundX = jewelBound.x;
			int jewelDrawBoundY = jewelBound.y;

			// Interpolate between jewel's location and destination for a distance of its progress to find draw location
			if( currentJewel->inTransit() )
			{
				int sourceX = jewelBound.x;
				int sourceY = jewelBound.y;
				int destX = jewelDest.x;
				int destY = jewelDest.y;
				float progress = currentJewel->getProgress();
				int deltaX = destX - sourceX;
				int deltaY = destY - sourceY;
				int xOffset = (int)(deltaX * progress);
				int yOffset = (int)(deltaY * progress);
				int visualX = sourceX + xOffset;
				int visualY = sourceY + yOffset;

				jewelDrawBoundX = jewelBound.x + (int)( ( jewelDest.x - jewelBound.x ) * currentJewel->getProgress() );
				jewelDrawBoundY = jewelBound.y + (int)( ( jewelDest.y - jewelBound.y ) * currentJewel->getProgress() );
			}

			// Jewel centres required to accomodate jewel surface variety in size
			int jewelX = jewelDrawBoundX + (int)(jewelBound.w * 0.5);
			int jewelY = jewelDrawBoundY + (int)(jewelBound.h * 0.5);

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
				default:
				break;
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

	// Initialise truetype font
	if ( TTF_Init() == -1 )
	{
		return false;
	}

	// Set window caption
	SDL_WM_SetCaption( "Jewel Matcher", NULL );

	// Seed random function with time to hide determinism
	srand((int)time(NULL));

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
	font = TTF_OpenFont( "#44v2.ttf", 28 );
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

// Checks number of sockets selected and prompts behaviour accordingly, switching jewels if two sockets are selected
bool performSocketSelection( int x, int y, Grid* &gameGrid )
{
	// Collect pointer to socket being upclicked
	Socket* upClickedSocket = gameGrid->getSocketAtCoordinates( x, y );
	if( upClickedSocket != NULL )
	{
		// Add socket to selection vector
		selectedSockets.push_back(upClickedSocket);
		int currentSelectionSize = (int)selectedSockets.size();
							
		// Vary behaviour, dependent on how many sockets are currently selected
		if( currentSelectionSize == 1 )
		{
			// A socket is selected
			printf("One socket selected");
		}
		else if( currentSelectionSize == 2 )
		{
			// Once two sockets are selected, attempt jewel exchange validation and execution
			// If selection was same socket twice, deselection should be the only action
			if( selectedSockets.at(0) != selectedSockets.at(1) )
			{	
				gameGrid->attemptJewelExchange( selectedSockets.at(0), selectedSockets.at(1) );

				// Check whether jewel switch has created color groups by attempting to detect and score them							
				int totalGroupsScored = gameGrid->scoreColorGroups( (gameGrid->findColorGroups()), gameScore );
				if( totalGroupsScored > 0 )
				{	
					selectedSockets.clear();
				}
				else
				{
					// Illegal move requires a switch back
					return false;
				}
			}
		}
		else if( currentSelectionSize > 2 )
		{
			// Should not be able to select more than two, so error has occurred
			std::cout << "Selection error has occured. " << selectedSockets.size() << " sockets were selected. Clearing selection." << std::endl;
			selectedSockets.clear();
		}
	}
	return true;
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

	SDL_Surface* titleMessage =	TTF_RenderText_Solid( font, "Jewel Matcher", textColor );
	SDL_Surface* pausedMessage = TTF_RenderText_Solid( font, "Paused - Press 'p' to Continue", textColor );
	SDL_Surface* gameOverMessage = TTF_RenderText_Solid( font, "Game Over", textColor );
	SDL_Surface* timerMessage = TTF_RenderText_Solid( font, "Time left: ", textColor );
	SDL_Surface* scoreMessage = TTF_RenderText_Solid( font, "Score:", textColor );

	//Apply image to screen
	apply_surface( 0, 0, backgroundImage, screen );

	// Construct Grid of Sockets and Jewels
	// Based on arbitary grid width provided, determine grid starting coordinates
	Grid* gameGrid = new Grid(GRID_SIZE, GRID_CENTRE_X, GRID_CENTRE_Y, CELL_W, CELL_H);

	// Pausing prevents game behaviours, stops timer, stops interaction other than unpausing, but continues draw calls
	bool isPaused = false;
	// Flag indicating whether game has finished
	bool gameOver = false;

	// If the game is animating someting, interaction is prevented
	bool gridReady = true;
	// Track whether (and where) an upClick has occurred
	bool upClickOccurred = false;
	int mUpClickX = 0;
	int mUpClickY = 0;
	// If an illegal move is made (no color groups formed), flag indicates they must be switched back
	bool legalMove = true;

	// Illegal moves incur a momentary pause before jewels are switched back
	// A counter will accumulate animationTime until 
	int switchBackTimerTarget = 11000;
	int switchBackTimerCounter = 0;

	// Game start time
	int gameStartTime = SDL_GetTicks();
	int totalTimeElapsed = 0;
	int currentTime = gameStartTime;
	// Track time from previous iteration, for calculating delta-time
	int prevTime = gameStartTime;
	// Change in time since last tick
	int deltaTime = 0;
	// Increase counter 'accumulatedTime' until it reaches threshold of fixedTimeStep, prompting movements/animations to be performed
	int accumulatedTime = 0;
	// Used to instill regularity in animation
	int fixedTimeStep = 1000;
	// Controls whether functions are fed 0 or fixedTimeStep as their deltaTime
	int animationTime = 0;

	//Pause
	while( quit == false )
	{
		if( !gameOver )
		{
			// Determine how much time has elapsed since last iteration
			currentTime = SDL_GetTicks();
			// Time elapsed is 
			totalTimeElapsed = currentTime - gameStartTime;
			deltaTime =  currentTime - prevTime;
			accumulatedTime += deltaTime;
			animationTime = 0;
			if( accumulatedTime >= fixedTimeStep )
			{
				animationTime = fixedTimeStep;
				// Accumulation variable can keep remainder after fixedTimeStep is taken away from it, as long as remainder is less than fixedTimeStep
				accumulatedTime = accumulatedTime % fixedTimeStep;
			}
		}

		// Event handling
		while( SDL_PollEvent( &event ) )
		{
			// When exiting the program
			if( event.type == SDL_QUIT )
			{
				//Exit
				quit = true;
			}

			// Track mouse coordinates
			if( event.type == SDL_MOUSEMOTION )
			{
				mouseX = event.motion.x;
				mouseY = event.motion.y;
			}

			// On key-up of 'p' key, pause the game
			if ( event.type == SDL_KEYUP )
			{
				if( event.key.keysym.sym == SDLK_p )
				{
					// Toggling pause
					isPaused = !isPaused;
				}
			}

			// User Game interaction - EVENTS WHICH OCCUR WHEN BOARD IS STATIC
			if ( event.type == SDL_MOUSEBUTTONUP )
			{
				// Click-down's don't matter as much as click-ups
				if( event.button.button == SDL_BUTTON_LEFT )
				{
					upClickOccurred = true;
					mUpClickX = event.button.x;
					mUpClickY = event.button.y;
				}
			}
		
		} // End of poll event clearing

		// Check if the game timer has run out yet
		if( totalTimeElapsed < totalGameLength )
		{
			// Pause inhibits user input and animation
			if( isPaused )
			{
				// Set message so user understands what state game is in and why interaction is halted.
				message = pausedMessage;

				// Up-Clicks occuring during paused game state are rejected
				upClickOccurred = false;
			}
			else
			{			
				message = titleMessage;

				// Reset flag before checking conditions
				gridReady = false;
				// Check if sockets are full
				gridReady = gameGrid->socketsAreFull( animationTime );
				// Check if jewels are moving
				gridReady = gameGrid->jewelsAreStatic( animationTime );

				// Only allow user mouse interaction if game is not handling grid animation etc
				if( !gridReady )
				{
					// If grid is not ready...
					// - prompt empty sockets to steal/generate jewels
					// - prompt jewels to move towards their destinations
				}
				else
				{
					// Animations have been resolved, grid is ready for input

					// Illegal moves require jewels to be switched back before the game can continue
					if( !legalMove )
					{
						switchBackTimerCounter += animationTime;
						if( switchBackTimerCounter >= switchBackTimerTarget )
						{	
							// Reset switchback delay variable
							switchBackTimerCounter = 0;

							// Invalid move by those currently in selection must be reversed
							gameGrid->attemptJewelExchange( selectedSockets.at(0), selectedSockets.at(1) );
							// Indicate switchback has been completed
							legalMove = true;
							// Deselect sockets involved in switch back
							selectedSockets.clear();
						}
					}
					else
					{
						// If an upClick occurred, check if it was within grid
						if( upClickOccurred )
						{ 
							if( gameGrid->withinBound( mUpClickX, mUpClickY ) ) 
							{
								// Reset the flag for the next upclick
								upClickOccurred = false;
								// Execute behaviour for selecting sockets at coordinates and handling resulting behaviour
								legalMove = performSocketSelection( mUpClickX, mUpClickY, gameGrid );							
							}
							else
							{							
								// Clicking outside of the grid is used for deselection
								selectedSockets.clear();
							}
						}
					}
				} // End of gridReady/!gridReady statement
			} // End of pause/!pause statement
		} 
		else
		{		 
			gameOver = true;
		}

		// Draw back game visuals
		apply_surface( 0, 0, backgroundImage, screen );
		drawGrid( gameGrid );

		// Display timer
		std::stringstream time; 
		time << "Time Left: " << (int)( (totalGameLength - totalTimeElapsed) / 1000.0f ) << " seconds";
		timerMessage = TTF_RenderText_Solid( font, time.str().c_str(), textColor );
		apply_surface( 15, 567, timerMessage, screen);

		// Display "Score:"		
		scoreMessage = TTF_RenderText_Solid( font, "Score:", textColor );
		apply_surface( 458, 460, scoreMessage, screen);
		// Display score value
		std::stringstream currentScore;
		currentScore << gameScore;
		scoreMessage = TTF_RenderText_Solid( font, currentScore.str().c_str(), textColor );
		int scoreXCentred = 495;
		// Actual x location must be offset for each numeral in the score
		int numeralCounter = 1;
		int tempScore = gameScore;
		while( tempScore > 0 )
		{
			tempScore = (int)( tempScore / 10 );
			numeralCounter++;
		}
		int characterWidth = 6;
		int offsetScoreX = scoreXCentred - ( ( numeralCounter / 2 ) * characterWidth ) ;
		if( numeralCounter > 1 )
		{
		//	offsetScoreX -= ( numeralCounter % 2 ) * characterWidth;
		}
		apply_surface( offsetScoreX, 540, scoreMessage, screen);

		if( gameOver )
		{
			// Draw score
			message = gameOverMessage;
		}

		// Refresh message
		if ( message != NULL )
		{			
			apply_surface( 400, 55, message, screen);
		}

		//Update screen
		if ( SDL_Flip( screen ) == -1 )
		{
			return 1;
		}
	}

	// Free-up surfaces and quit SDL
	clean_up();
    
    return 0;    
}

