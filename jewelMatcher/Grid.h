#ifndef GRID_H
#define GRID_H

#include "SDL.h"
#include <vector>

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
		Grid::Grid(int newGridWidth, int newCentreX, int newCentreY, int newCellWidth, int newCellHeight);
		Grid::~Grid();

		// Create sockets
		void Grid::generateSockets();
		// Fill all sockets with jewels
		void Grid::populateSockets();

		int getGridWidth();

		Socket* getSocketAtRowColumn( int row, int column );

		// Making the coordinate relative to the grid, and dividing by a cell dimension, provides column (or row) number
		int Grid::getColumnForSocketIndex(int socketIndex);
		int Grid::getRowForSocketIndex(int socketIndex);
		
		int convertRowColumnToSocketIndex(int row, int column);
		
		int Grid::getColumnFromXCoord(int x);
		int Grid::getRowFromYCoord(int y);

		SDL_Rect Grid::calcSocketBoundFromIndex(int socketIndex);

		std::vector<Socket*>::iterator Grid::getSocketsBeginning();
		std::vector<Socket*>::iterator Grid::getSocketsEnd();

		bool Grid::withinBound(int x, int y);

		// Given screen coordinates checked for grid-validity, then used to return a pointer to socket at that location
		Socket* Grid::getSocketAtCoordinates(int x, int y);

		// Validity of exchange checked, then jewels instructed to swap places
		bool Grid::attemptJewelExchange( Socket* firstSocket, Socket* secondSocket );

		// Return true if socket locations are a legal switch
		bool Grid::checkSocketAdjacency( Socket* firstSocket, Socket* secondSocket );

		// Returns true if all jewels complete their moves, false if some are still in transit
		bool Grid::moveJewelsToDestinations();
};

#endif