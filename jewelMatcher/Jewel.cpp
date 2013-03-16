#include "Jewel.h"

Jewel::Jewel(){
}

Jewel::Jewel(SDL_Rect newJewelBound): 
	jewelBound( newJewelBound ), 
	destination( newJewelBound ),
	progress( 1.0 ),
	travelSpeed( 0.00005 ),
	jewelType( getRandomJewelType() )
{
}

Jewel::Jewel(SDL_Rect newJewelBound, char newJewelType):
	jewelBound( newJewelBound ), 
	destination( newJewelBound ),
	progress( 1.0 ),
	travelSpeed( 0.00005 ),
	jewelType( newJewelType )
{
}

char Jewel::getRandomJewelType() const
{
	float r = (float)( rand() / (float)(RAND_MAX));
	char chosenJewelType = 'p';

	if ( r < 0.2 )
	{
		chosenJewelType = 'r';
	}
	else if ( r <= 0.4 )
	{
		chosenJewelType = 'b';
	}
	else if ( r <= 0.6 )
	{
		chosenJewelType = 'y';
	}
	else if ( r <= 0.8 )
	{
		chosenJewelType = 'g';
	}
	else
	{
		chosenJewelType = 'p';
	}

	return chosenJewelType;
}

SDL_Rect Jewel::getDestination() const
{
	return destination;
}

float Jewel::getProgress() const
{
	return progress;
}

SDL_Rect Jewel::getJewelBound() const
{
	return jewelBound;
}

char Jewel::getJewelType() const{
	return jewelType;
};

bool Jewel::inTransit() const
{
	if( progress == 1.0 )
	{
		// jewel is not moving (is at destination)
		return false;
	}
	return true;
}

void Jewel::setNewDestination( SDL_Rect newDest )
{
	destination = newDest;
	if( ( destination.x == jewelBound.x ) && ( destination.y == jewelBound.y ) )
	{
		// Already at destination
		progress = 1.0;
	}
	else
	{
		progress = 0.0;
	}
}

bool Jewel::moveToDestination( int const deltaTime )
{
	// moveToDestination is the only way to increase progress
	float thisProgress = travelSpeed * deltaTime;
	progress += thisProgress;
	float totalProgress = progress;
	// progress must be clamped to max of 1
	if( progress >= 1.0 )
	{
		progress = 1.0;
		jewelBound = destination;
		return true;
	}
	return false;
}
