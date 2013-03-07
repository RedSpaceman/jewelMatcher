#include "Jewel.h"
#include <stdlib.h>

#include "SDL.h"
#include "SDL_image.h"

Jewel::Jewel(){
}

Jewel::Jewel(int newX, int newY){
	x = newX;
	y = newY;
	jewelType = getRandomJewelType();
}

Jewel::Jewel(int newX, int newY, char newJewelType){
	x = newX;
	y = newY;
	jewelType = newJewelType;
}

int Jewel::getX()
{
	return x;
};

int Jewel::getY()
{
	return y;
};

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