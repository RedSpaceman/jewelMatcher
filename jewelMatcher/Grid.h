#ifndef GRID_H
#define GRID_H

#include "SDL.h"
#include <vector>

#include "ColorGroup.h"
#include "Socket.h"

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
		Grid(int newGridWidth, int newCentreX, int newCentreY, int newCellWidth, int newCellHeight);
		~Grid();

		// Create sockets
		void generateSockets();
		// Fill all sockets with jewels
		void populateSockets();
		// Randomises jewels in sockets that belong to existing color groups
		void attemptColorGroupScramble(std::vector<ColorGroup*> &colorGroups);

		int getGridWidth();

		Socket* getSocketAtRowColumn( int row, int column );

		// Making the coordinate relative to the grid, and dividing by a cell dimension, provides column (or row) number
		int getColumnForSocketIndex(int socketIndex);
		int getRowForSocketIndex(int socketIndex);
		
		int convertRowColumnToSocketIndex(int row, int column);
		
		int getColumnFromXCoord(int x);
		int getRowFromYCoord(int y);

		SDL_Rect calcSocketBoundFromIndex(int socketIndex);

		std::vector<Socket*>::iterator getSocketsBeginning();
		std::vector<Socket*>::iterator getSocketsEnd();

		// Check if the given coordinates are inside the grid's boundary
		bool Grid::withinBound(int x, int y);

		// Given screen coordinates checked for grid-validity, then used to return a pointer to socket at that location
		Socket* getSocketAtCoordinates(int x, int y);

		// Validity of exchange checked, then jewels instructed to swap places
		bool attemptJewelExchange( Socket* firstSocket, Socket* secondSocket );

		// Return true if socket locations are a legal switch
		bool checkSocketAdjacency( Socket* firstSocket, Socket* secondSocket );

		// Returns true if all jewels complete their moves, false if some are still in transit
		bool moveJewelsToDestinations();

		// Searches the grid for locations where jewels of the same colour form lines of greater than three
		std::vector<ColorGroup*> findColorGroups();

		// Receives detected color groups, removes the jewels from their sockets and scores them, returning number of groups scored
		int scoreColorGroups( std::vector<ColorGroup*> &validGroups, int &gameScore);

		// True if sockets all have jewels, collapse board and return false if not
		bool socketsAreFull( int deltaTime );

		// True if no jewels are in transit
		bool jewelsAreStatic( int deltaTime );

};

#endif