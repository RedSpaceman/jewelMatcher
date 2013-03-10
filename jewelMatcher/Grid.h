#ifndef GRID_H
#define GRID_H

#include "SDL.h"
#include <vector>

#include "Socket.h"

class Grid{
	private: 
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
		int Grid::getRowForSocketIndex(int socketIndex);
		int Grid::getColumnForSocketIndex(int socketIndex);

		int convertRowColumnToSocketIndex(int row, int column);

		int Grid::getSocketXCentreFromIndex(int socketIndex);
		int Grid::getSocketYCentreFromIndex(int socketIndex);
		SDL_Rect Grid::calcSocketBoundFromIndex(int socketIndex);


		std::vector<Socket*>::iterator Grid::getSocketsBeginning();
		std::vector<Socket*>::iterator Grid::getSocketsEnd();
};

#endif