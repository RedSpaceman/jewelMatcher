#ifndef JEWEL_H
#define JEWEL_H

#include <stdlib.h>

#include "SDL.h"
#include "SDL_image.h"

class Jewel
{
	private:
		SDL_Rect jewelBound;
		SDL_Rect destination;
		// Progress between 0 and 1 represents percentage of distance covered bound and destination, for interpolation
		float progress;
		// Amount of time taken to move a full '1' distance of progress
		float travelSpeed;
		char jewelType;
		// Used for randomly designating a color to a jewel
		char getRandomJewelType() const;
	public:	
		
		/* Constructors */
		Jewel();
		Jewel(SDL_Rect newJewelBound);
		Jewel(SDL_Rect newJewelBound, char newJewelType);

		/* Getter Functions */
		SDL_Rect getDestination() const;
		// Returns proportion of distance jewel has travelled between current boundary and destination
		float getProgress() const;
		// Returns jewel's boundary
		SDL_Rect getJewelBound() const;
		// Returns jewel's color
		char getJewelType() const;

		// If jewel progress is less than 1 it is only part way to its destination and still moving
		bool inTransit() const;

		/* Setter Functions */
		void setNewDestination(SDL_Rect const newDest);

		// Moves jewel from bound location towards destination, returning true if destination is reached
		bool moveToDestination( int const deltaTime );


};

#endif


