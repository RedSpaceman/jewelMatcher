#ifndef JEWEL_H
#define JEWEL_H

#include <stdlib.h>

#include "SDL.h"
#include "SDL_image.h"

class Jewel {

		SDL_Rect jewelBound;
		char jewelType;
		char getRandomJewelType();
	public:	
		
		Jewel();
		Jewel(SDL_Rect newJewelBound);
		Jewel(SDL_Rect newJewelBound, char newJewelType);

		SDL_Rect getJewelBound();
		char getJewelType();		
};

#endif


