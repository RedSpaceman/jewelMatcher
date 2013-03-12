#ifndef JEWEL_H
#define JEWEL_H

#include <stdlib.h>

#include "SDL.h"
#include "SDL_image.h"

class Jewel {

		SDL_Rect jewelBound;
		SDL_Rect destination;
		char jewelType;
		char getRandomJewelType();
	public:	
		
		Jewel();
		Jewel(SDL_Rect newJewelBound);
		Jewel(SDL_Rect newJewelBound, char newJewelType);

		void Jewel::setNewDestination(SDL_Rect newDest);

		SDL_Rect getJewelBound();
		char getJewelType();	

		// Moves jewel from bound location towards destination, returning true if destination is reached
		bool Jewel::moveToDestination();

		// If the jewel's bound and destination do not match, the jewel must be moving
		bool Jewel::inTransit();
};

#endif


