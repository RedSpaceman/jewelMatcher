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
	int boardGenNumber = 1;
	bool acceptBoard = false;
	while( !acceptBoard )
	{
		// Instruct each socket to generate a new jewel
		int totalSockets = gridWidth*gridWidth;
		for( int i = 0; i < totalSockets; i++)
		{
			// Use socket boundary as jewel boundary
			SDL_Rect jewelBound = calcSocketBoundFromIndex(i);
			sockets.at(i)->generateJewel( jewelBound );
		}

		std::vector<ColorGroup*> colorGroups = findColorGroups();
		// Check board for pre-existing color groups
		if( colorGroups.size() > 0 )
		{
			// Delete board and try again
			boardGenNumber++;
			
			// Scramble color group sockets
			attemptColorGroupScramble(colorGroups);
			// Verify if color group scrambling removed all color groups
			colorGroups = findColorGroups();
			if( colorGroups.size() > 0 )
			{
				acceptBoard = true;
			}
		}
		else
		{
			// Generated board is acceptable!
			acceptBoard = true;
		}
	}
}

void Grid::attemptColorGroupScramble(std::vector<ColorGroup*> &colorGroups)
{
	// Attempt to scramble existing color group in an attempt to accelerate grid regeneration
	for(std::vector<ColorGroup*>::iterator colorGroupItr = colorGroups.begin(); colorGroupItr != colorGroups.end(); colorGroupItr++)
	{
		std::vector<Socket*>::iterator socketsBegin = (*colorGroupItr)->getGroupSocketsBeginning();
		std::vector<Socket*>::iterator socketsEnd = (*colorGroupItr)->getGroupSocketsEnd();
		// Regenerate jewel for each socket in colorGroup
		for(std::vector<Socket*>::iterator socketItr = socketsBegin; socketItr != socketsEnd; socketItr++)
		{
			(*socketItr)->generateJewel( (*socketItr)->getSocketBound() );
		}
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

std::vector<ColorGroup*> Grid::findColorGroups()
{
	// Definition: Color groups are like-colored jewels in groups >=3 in a line
	int minimumGroupLength = 3;

	// Vector used to track all ColorGroups found so they can be scored together at end of search
	std::vector<ColorGroup*> colorGroups;
	// Next potential ColorGroup in progress stored in nextColorGroup
	std::vector<Socket*> matchingSockets;

	// Flag to indicate current potential colorGroup is complete (mismatch or end of row encountered)
	bool scoreAndReset = false;
	char currentGroupColor = 'z';

	// Run along rows, counting consecutive colours
	for( int socketIndex = 0; socketIndex < sockets.size(); socketIndex++ )
	{
		Socket* nextSocket = sockets.at( socketIndex );

		// If a new row, create a new color group
		if( ( socketIndex % gridWidth ) == 0 )
		{
			matchingSockets.clear();
		}

		char nextJewelColor = nextSocket->getCurrentJewelType();
		
		// Color match checking occurs during attempt to add socket to current group
		// Also check whether socket is last in row, which causes end of current color group
		bool endOfRow = ( ( ( socketIndex + 1 ) % gridWidth ) == 0 );
		if( matchingSockets.size() == 0 )
		{
			// This socket is first in new set, so push it in
			matchingSockets.push_back( nextSocket );
			currentGroupColor = nextSocket->getCurrentJewelType();
		}
		else
		{
			// If color matches existing colors of first entry, push new socket into set
			if( nextJewelColor == currentGroupColor )
			{
				matchingSockets.push_back( nextSocket );
				currentGroupColor = nextSocket->getCurrentJewelType();
			}
			else
			{
				scoreAndReset = true;
			}
		}
		int nextGroupSize = matchingSockets.size();
		// Check if end of row
		if( endOfRow )
		{
			scoreAndReset = true;
		}

		// If end of row, clear the matchingSockets vector
		if( scoreAndReset )
		{
			scoreAndReset = false;
			if( matchingSockets.size() >= minimumGroupLength )
			{
				colorGroups.push_back( new ColorGroup( matchingSockets ) );
				printf("points!");
			}
			// Prepare for next potential color group
			matchingSockets.clear();
			// Socket that caused color match failure must be first entry in next potential group
			if( !endOfRow )
			{
				matchingSockets.push_back( nextSocket );
				currentGroupColor = nextSocket->getCurrentJewelType();
			}
		}
	}
	
	// Check for vertical groups in each columns
	/*
	for( int columnIndex = 0; columnIndex < gridWidth; columnIndex++ )
	{
		// For each column there is the same number of socket indices between the top and bottom socket
		int bottomSocketIndexInColumn = columnIndex + ( gridWidth * ( gridWidth - 1 ));
		// For each consecutive column-wise entry, socketIndex increases by gridWidth
		for( int socketIndex = columnIndex; socketIndex < bottomSocketIndexInColumn; socketIndex += gridWidth )
		{
			// Color match checking occurs during attempt to add socket to current group
			// Also check whether socket is last in column, which causes end of current color group
			bool successfulGroupAddition = nextColorGroup->addSocketToGroup( sockets.at(socketIndex) );
			if( (successfulGroupAddition) || socketIndex == bottomSocketIndexInColumn )
			{
				// Check if minimum length for scoring was reached
				if( nextColorGroup->getGroupSize() >= minimumGroupLength )
				{
					colorGroups.push_back( nextColorGroup );
					printf("points!");
				}
			}
		}
	}
	*/
	return colorGroups;
}
