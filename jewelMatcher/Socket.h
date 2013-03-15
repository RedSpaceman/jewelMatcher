#ifndef SOCKET_H
#define SOCKET_H

#include "Jewel.h"

class Socket
{
	private:
		SDL_Rect socketBound;
		Jewel* currentJewel;
	public:
		Socket( SDL_Rect );

		// If newJewel is valid, stores and returns true
		bool setJewel(Jewel* newJewel);

		SDL_Rect getSocketBound();
		int getWidth();
		int getHeight();

		Jewel* getCurrentJewel();
		char getCurrentJewelType();
		
		// Indicates if socket currently contains a jewel
		bool containsJewel();

		void generateJewel( SDL_Rect newJewelBound );
		// Allows jewel to be spawned in initial location relative to socket coordinates
		void generateOffsetJewel( int x, int y );

		Jewel* relinquishJewel();

		// Set currentJewel to NULL, discarding the socket's contents
		void discardJewel();

		// If the socket contains a jewel, the Rect param will be jewel's new target location, returning false if no jewel found
		bool setCurrentJewelDestination( SDL_Rect newDestination );

		// Moves jewel towards destination, returning true upon reaching it
		bool moveJewelToDestination( int deltaTime );
};

#endif