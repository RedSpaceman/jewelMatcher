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
		
		/* Constructors */
		Jewel();
		Jewel(SDL_Rect newJewelBound);
		Jewel(SDL_Rect newJewelBound, char newJewelType);

		/* Getter Functions */
		SDL_Rect getDestination();
		// Returns proportion of distance jewel has travelled between current boundary and destination
		float getProgress();
		// Returns jewel's boundary
		SDL_Rect getJewelBound();
		// Returns jewel's color
		char getJewelType();	

		/* Setter Functions */
		void setNewDestination(SDL_Rect newDest);

		// If the jewel's bound and destination do not match, the jewel must be moving
		bool inTransit();



		// Moves jewel from bound location towards destination, returning true if destination is reached
		bool moveToDestination( int deltaTime );


};

#endif


