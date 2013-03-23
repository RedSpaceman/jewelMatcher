#ifndef GRID_H
#define GRID_H

#include "SDL.h"
#include <vector>

#include "../include/ColorGroup.h"
#include "../include/Socket.h"

class Grid{
	private: 
		// Grid width is the number of grid cells along one side
		int gridWidth;

		int cellWidth;
		int cellHeight;

		SDL_Rect gridBound;
		
		// Vector of sockets
		std::vector<Socket*> sockets;

	public:
		/* Constructor and Destructor */
		Grid( int newGridWidth, int newCentreX, int newCentreY, int newCellWidth, int newCellHeight );
		~Grid();

		/* Grid Setup */
		// Create sockets
		void generateSockets();
		// Fill all sockets with jewels
		void populateSockets();
		// Randomises jewels in sockets that belong to existing color groups
		void attemptColorGroupScramble( std::vector<ColorGroup*> &colorGroups );

		/* Getters */
		int getGridWidth() const;
		SDL_Rect calcSocketBoundFromIndex(int socketIndex);
	
		/* Socket Row/Column Calculators */
		// Making the coordinate relative to the grid, and dividing by a cell dimension, provides column (or row) number
		int getColumnForSocketIndex(int const socketIndex) const;
		int getRowForSocketIndex(int const socketIndex) const;		
		int getColumnFromXCoord(int const x) const;
		int getRowFromYCoord(int const y) const;

		/* Socket Access Functions */
		int convertRowColumnToSocketIndex(int const row, int const column) const;
		Socket* getSocketAtRowColumn( int const row, int const column ) const;
		// Given screen coordinates checked for grid-validity, then used to return a pointer to socket at that location
		Socket* getSocketAtCoordinates(int x, int y);

		std::vector<Socket*>::iterator getSocketsBeginning();
		std::vector<Socket*>::iterator getSocketsEnd();

		/* Validity Checking */
		// Check if the given coordinates are inside the grid's boundary
		bool Grid::withinBound(int x, int y) const;
		// Return true if socket locations are a legal switch
		bool checkSocketAdjacency( Socket const* firstSocket, Socket const* secondSocket );

		/* Jewel Moving */
		// Validity of exchange checked, then jewels instructed to swap places
		bool attemptJewelExchange( Socket* firstSocket, Socket* secondSocket );

		/* Scoring Groups */
		// Searches the grid for locations where jewels of the same colour form lines of greater than three
		std::vector<ColorGroup*> findColorGroups();
		// Receives detected color groups, removes the jewels from their sockets and scores them, returning number of groups scored
		int scoreColorGroups( std::vector<ColorGroup*> &validGroups, int &gameScore);

		/* Checking Gamestate */
		// True if sockets all have jewels, collapse board and return false if not
		bool socketsAreFull( int deltaTime );
		// True if no jewels are in transit
		bool jewelsAreStatic( int deltaTime ) const;

};

#endif