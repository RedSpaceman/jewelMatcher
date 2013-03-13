#include "Jewel.h"

Jewel::Jewel(){
}

Jewel::Jewel(SDL_Rect newJewelBound)
{
	jewelBound = newJewelBound;
	destination = jewelBound;

	progress = 1.0;
	travelSpeed = 0.000001;

	jewelType = getRandomJewelType();
}

Jewel::Jewel(SDL_Rect newJewelBound, char newJewelType)
{
	jewelBound = newJewelBound;
	destination = jewelBound;
	
	progress = 1.0;
	travelSpeed = 0.000001;

	jewelType = newJewelType;
}

SDL_Rect Jewel::getDestination()
{
	return destination;
}

void Jewel::setNewDestination(SDL_Rect newDest)
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

float Jewel::getProgress()
{
	return progress;
}

SDL_Rect Jewel::getJewelBound()
{
	return jewelBound;
}

char Jewel::getJewelType(){
	return jewelType;
};

char Jewel::getRandomJewelType(){
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

bool Jewel::moveToDestination( int deltaTime )
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

bool Jewel::inTransit()
{
	if( progress == 1.0 )
	{
		// jewel is not moving
		return false;
	}
	return true;
}