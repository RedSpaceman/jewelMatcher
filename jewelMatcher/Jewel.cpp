#include "Jewel.h"

Jewel::Jewel(){
}

Jewel::Jewel(SDL_Rect newJewelBound)
{
	jewelBound = newJewelBound;
	destination = jewelBound;
	jewelType = getRandomJewelType();
}

Jewel::Jewel(SDL_Rect newJewelBound, char newJewelType)
{
	jewelBound = newJewelBound;
	destination = jewelBound;
	jewelType = newJewelType;
}

void Jewel::setNewDestination(SDL_Rect newDest)
{
	destination = newDest;
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

bool Jewel::moveToDestination()
{
	// TODO: Make movement non-instant
	jewelBound = destination;
	return true;
}

bool Jewel::inTransit()
{
	// Only care about coordinates, rather than w & h as well
	if( jewelBound.x == destination.x 
		&& jewelBound.y == destination.y )
	{
		// jewel is not moving
		return false;
	}
	return true;
}