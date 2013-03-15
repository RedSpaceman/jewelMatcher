#ifndef JEWEL_H
#define JEWEL_H

#include <stdlib.h>

#include "SDL.h"
#include "SDL_image.h"

class Jewel {

		SDL_Rect jewelBound;
		SDL_Rect destination;
		// Progress between 0 and 1 represents percentage of distance covered bound and destination, for interpolation
		float progress;
		// Amount of time taken to move a full '1' distance of progress
		float travelSpeed;
		char jewelType;
		char getRandomJewelType();
	public:	
		
		Jewel();
		Jewel(SDL_Rect newJewelBound);
		Jewel(SDL_Rect newJewelBound, char newJewelType);

		SDL_Rect getDestination();
		void setNewDestination(SDL_Rect newDest);

		float getProgress();

		SDL_Rect getJewelBound();
		char getJewelType();	

		// Moves jewel from bound location towards destination, returning true if destination is reached
		bool moveToDestination( int deltaTime );

		// If the jewel's bound and destination do not match, the jewel must be moving
		bool inTransit();
};

#endif


