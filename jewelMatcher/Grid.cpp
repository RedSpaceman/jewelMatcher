#include "Grid.h"

Grid::Grid(int newGridWidth, int newCentreX, int newCentreY, int newCellWidth, int newCellHeight){
	gridWidth = newGridWidth;
	cellWidth = newCellWidth;
	cellHeight = newCellHeight;

	// If gridWidth is odd, gridWidth%2 allows additional half-width shift to centre central socket
	gridBound.x = (int)(newCentreX-(cellWidth*(gridWidth*0.5))) - (int)((gridWidth%2) * (cellWidth*0.5));
	gridBound.y = (int)(newCentreY-(cellHeight*(gridWidth*0.5))) - (int)((gridWidth%2) * (cellHeight*0.5));
	gridBound.w = gridWidth * cellWidth;
	gridBound.h = gridWidth * cellHeight;

	// Allocate the number of elements required
	generateSockets();
	populateSockets();
}

Grid::~Grid()
{
	// TODO: Must delete sockets
}

void Grid::generateSockets()
{
	// Generate gridWidth^2 sockets
	int totalSockets = gridWidth*gridWidth;
	for( int i = 0; i < totalSockets; i++)
	{
		SDL_Rect socketBound = calcSocketBoundFromIndex(i);
		sockets.push_back( new Socket( socketBound ) );
	}
}

void Grid::populateSockets()
{
	// Instruct each socket to generate a new jewel
	int totalSockets = gridWidth*gridWidth;
	for( int i = 0; i < totalSockets; i++)
	{
		// Use socket boundary as jewel boundary
		SDL_Rect jewelBound = calcSocketBoundFromIndex(i);
		sockets.at(i)->generateJewel( jewelBound );
	}
}

int Grid::getGridWidth(){
	return gridWidth;
}

Socket* Grid::getSocketAtRowColumn( int row, int column )
{
	return sockets.at( convertRowColumnToSocketIndex( row, column ) );
}

int Grid::convertRowColumnToSocketIndex(int row, int column)
{
	// Socket index starts in top left of grid (0) and counts rightwards, starting at the left for each new row

	// Convert x and y into a sockets index
	// Testing logic (gW=5)
	//  first column, first row(idx 0): 1,1 -> 0+0
	//  fourth column, second row(idx 8): 4,2 -> 3+(1*5)=8
	return (( column - 1 ) + (( row - 1 ) * gridWidth ));
}

int Grid::getColumnForSocketIndex(int socketIndex)
{
	return ( socketIndex % gridWidth ) + 1;
}

int Grid::getRowForSocketIndex(int socketIndex)
{
	return ((int)( socketIndex / gridWidth ) + 1 );
}


int Grid::getColumnFromXCoord(int x)
{
	return ( (x - gridBound.x) / cellWidth) + 1;
}

int Grid::getRowFromYCoord(int y)
{
	return ( (y - gridBound.y) / cellHeight ) + 1;
}

SDL_Rect Grid::calcSocketBoundFromIndex(int socketIndex)
{
	SDL_Rect socketBound;
	socketBound.x = gridBound.x + ( cellWidth * ( getColumnForSocketIndex(socketIndex) - 1 ) );
	socketBound.y = gridBound.y + ( cellHeight * ( getRowForSocketIndex(socketIndex) - 1 ) ); 
	socketBound.w = cellWidth;
	socketBound.h = cellHeight;

	return socketBound;
}

std::vector<Socket*>::iterator Grid::getSocketsBeginning()
{
	return sockets.begin();
}

std::vector<Socket*>::iterator Grid::getSocketsEnd()
{
	return sockets.end();
}

// Check if the given coordinates are inside the grid's boundary
bool Grid::withinBound(int x, int y)
{
	if( ( x >= gridBound.x ) && x <= gridBound.x + gridBound.w )
	{
		if( ( y >= gridBound.y ) && ( y <= gridBound.y + gridBound.h ) )
		{
			return true;
		}
	}
	return false;
}

Socket* Grid::getSocketAtCoordinates(int x, int y)
{
	Socket* selectedSocket = NULL;
	
	if( withinBound( x, y ) )
	{
		int column = getColumnFromXCoord(x);
		int row = getRowFromYCoord(y);
		selectedSocket = getSocketAtRowColumn(row, column);
	}
	return selectedSocket;
}

bool Grid::checkSocketAdjacency( Socket* firstSocket, Socket* secondSocket )
{
	SDL_Rect firstLoc = firstSocket->getSocketBound();
	SDL_Rect secondLoc = secondSocket->getSocketBound();

	if( firstLoc.x == secondLoc.x )
	{
		// Sockets are vertically aligned
		if( ( secondLoc.y == firstLoc.y - cellHeight ) || ( secondLoc.y == firstLoc.y + cellHeight ) )
		{
			// Cells are vertically adjacent
			return true;
		}
	}
	else if( firstLoc.y == secondLoc.y )
	{
		// Sockets are horizontally aligned
		if( ( secondLoc.x == firstLoc.x - cellWidth ) || ( secondLoc.x == firstLoc.x + cellWidth ) )
		{
			// Cells are horizontally adjacent
			return true;
		}
	}
	// Note: If locations are identical, false is returned
	return false;
}

bool Grid::attemptJewelExchange( Socket* firstSocket, Socket* secondSocket )
{
	// Check sockets are adjacent
	if( checkSocketAdjacency( firstSocket, secondSocket ) )
	{
		// Switch jewel pointers between sockets
		Jewel* firstJewel = firstSocket->getCurrentJewel();
		firstSocket->setJewel(secondSocket->getCurrentJewel());
		secondSocket->setJewel(firstJewel);
		if( firstSocket->getCurrentJewel() != NULL && secondSocket->getCurrentJewel() != NULL)
		{
			// Jewel ownership switched successfully
			// Update jewel locations by setting new destinations
			firstSocket->getCurrentJewel()->setNewDestination(firstSocket->getSocketBound());
			secondSocket->getCurrentJewel()->setNewDestination(secondSocket->getSocketBound());
			return true;
		}
		else
		{
			// An error has occured during pointer exchange
			return false;
		}
	}
	// Sockets were not adjacent, exchange failed
	return false;
}

