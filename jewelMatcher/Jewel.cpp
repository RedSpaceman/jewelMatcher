#include "Jewel.h"

Jewel::Jewel(){
}

Jewel::Jewel(SDL_Rect newJewelBound)
{
	jewelBound = newJewelBound;
	jewelType = getRandomJewelType();
}

Jewel::Jewel(SDL_Rect newJewelBound, char newJewelType)
{
	jewelBound = newJewelBound;
	jewelType = newJewelType;
}

SDL_Rect Jewel::getJewelBound()
{
	return jewelBound;
}

char Jewel::getJewelType(){
	return jewelType;
};

char Jewel::getRandomJewelType(){
	float r = (float)rand() / (float)RAND_MAX;
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