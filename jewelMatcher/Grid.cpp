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

		// Check board for pre-existing color groups
		if( findColorGroups() > 0 )
		{
			// Delete board and try again
			boardGenNumber++;
		}
		else
		{
			// Generated board is acceptable!
			acceptBoard = true;
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

int Grid::findColorGroups()
{
	// Definition: Color groups are like-colored jewels in groups >=3 in a line

	// Counter for total point-scoring groups found
	int totalColorGroups = 0;
	// Counter for how many jewels belong to the color group currently being tracked
	int jewelsInCurrentGroup = 0;
	// Color next jewel must match to be part of the current color group (default to non-colour value)
	char currentGroupColor = 'z';
	// Flag to indicate whether the current group has ended (end of row or non-match found)
	bool scoreAndReset = false;

	// Run along rows, counting consecutive colours
	for( int socketIndex = 0; socketIndex < sockets.size(); socketIndex++)
	{
		char nextJewelColor = sockets.at( socketIndex )->getCurrentJewelType();
		
		// Check color match, ensuring next socket isn't the first of a new row
		if( nextJewelColor == currentGroupColor )
		{
			// Added this jewel to the current color group
			jewelsInCurrentGroup++;
		}
		else
		{
			// Jewels did not match, so current color group ends
			scoreAndReset = true;
		}

		// Check if socket is last on the row
		if( (( socketIndex + 1 ) % gridWidth ) == 0 )
		{
			// End of row reached
			scoreAndReset = true;
		}

		// If a match fails, or the end of a row is reached, 
		if( scoreAndReset )
		{
			// Check if existing color group was point scoring size
			if( jewelsInCurrentGroup >= 3 )
			{
				// Point scoring group was formed 
				totalColorGroups++;
				// TODO: Award points
				printf("points!");
			}
			// Reset group tracking using current color, current jewel counts as first in next potential group
			jewelsInCurrentGroup = 1;
			currentGroupColor = nextJewelColor;
			scoreAndReset = false;
		}
	}
	
	// Check for vertical groups in each columns
	jewelsInCurrentGroup = 0;
	currentGroupColor = 'z';
	scoreAndReset = false;
	for( int columnIndex = 0; columnIndex < gridWidth; columnIndex++ )
	{
		// For each column there is the same number of socket indices between the top and bottom socket
		int bottomSocketIndexInColumn = columnIndex + ( gridWidth * ( gridWidth - 1 ));
		// For each consecutive column-wise entry, socketIndex increases by gridWidth
		for( int socketIndex = columnIndex; socketIndex < bottomSocketIndexInColumn; socketIndex += gridWidth )
		{
			char nextJewelColor = sockets.at(socketIndex)->getCurrentJewelType();
			
			// Check for jewel color match
			if( currentGroupColor == nextJewelColor )
			{
				jewelsInCurrentGroup++;
			}
			else
			{
				scoreAndReset = true;
			}

			// Check if socket is bottom in column
			if( socketIndex == bottomSocketIndexInColumn )
			{
				scoreAndReset = true;
			}

			if( scoreAndReset )
			{
				// Check if existing color group was point scoring size
				if( jewelsInCurrentGroup >= 3 )
				{
					// Point scoring group was formed 
					totalColorGroups++;
					// TODO: Award points
					printf("points!");
				}
				// Reset group tracking using current color, current jewel counts as first in next potential group
				jewelsInCurrentGroup = 1;
				currentGroupColor = nextJewelColor;
				scoreAndReset = false;
			}
		}
	}
	return totalColorGroups;
}
