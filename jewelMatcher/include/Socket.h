#ifndef SOCKET_H
#define SOCKET_H

#include "../include/Jewel.h"

class Socket
{
	private:
		SDL_Rect socketBound;
		Jewel* currentJewel;
	public:

		/* Constructor and Destructor */
		Socket( SDL_Rect );
		~Socket();

		/* Getter Functions */
		SDL_Rect getSocketBound() const;

		Jewel* getCurrentJewel() const;
		char getCurrentJewelType() const;
		
		// Returns true if socket currently contains a jewel
		bool containsJewel() const;

		/* Jewel Insertion */
		// If newJewel is valid, stores and returns true
		bool setJewel(Jewel* newJewel);
		// Generates a random jewel for the socket, discarding previous one if present
		void generateJewel( SDL_Rect const newJewelBound );
		// Allows jewel to be spawned in initial location relative to socket coordinates, used for jewels dropping in above grid
		void generateOffsetJewel( int const x, int const y );

		/* Jewel Removal */
		// When called by another socket, this socket must hand-over its jewel
		Jewel* relinquishJewel();
		// Set currentJewel to NULL, discarding the socket's contents
		void discardJewel();

		/* Jewel Animation */
		// If the socket contains a jewel, the Rect param will be jewel's new target location, returning false if no jewel found
		bool setCurrentJewelDestination( SDL_Rect const newDestination );
		// Moves jewel towards destination, returning true upon reaching it
		bool moveJewelToDestination( int const deltaTime );
};

#endif