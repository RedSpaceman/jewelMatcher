#include "Grid.h"

Grid::Grid(int newGridWidth, int newCentreX, int newCentreY, int newCellWidth, int newCellHeight):
	gridWidth( newGridWidth ),
	cellWidth( newCellWidth ),
	cellHeight( newCellHeight )
{
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
		for( int i = 0; i < totalSockets; i++ )
		{
			Socket* socket = sockets.at( i );
			// Use socket boundary as jewel boundary
			socket->generateJewel( socket->getSocketBound() );
		}

		// Check board for pre-existing color groups
		std::vector<ColorGroup*> colorGroups = findColorGroups();
		if( !colorGroups.empty() )
		{
			// Delete board and try again, counting number of successive boards for diagnostic purposes
			boardGenNumber++;
			
			// Scramble color group sockets
			attemptColorGroupScramble(colorGroups);
			// Verify if color group scrambling removed all color groups
			colorGroups = findColorGroups();
			if( colorGroups.size() == 0 )
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
		Jewel* secondJewel = secondSocket->getCurrentJewel();

		firstSocket->setJewel( secondJewel );
		secondSocket->setJewel( firstJewel );
		if( firstSocket->getCurrentJewel() != NULL && secondSocket->getCurrentJewel() != NULL)
		{
			// Jewel ownership switched successfully
			// Update jewel locations by setting new destinations
			firstSocket->setCurrentJewelDestination( firstSocket->getSocketBound() );
			secondSocket->setCurrentJewelDestination( secondSocket->getSocketBound() );

			return true;
		}
	}
	// Sockets were not adjacent, or exchange failed
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
	bool colorGroupEnded = false;
	// Flag to indicate that current socket should be inserted into a new color group (occurs only when current group ends)
	bool addSocketToNextGroup = false;
	// Track color of current group so it can be compared to next socket in row (default non-color 'z')
	char currentGroupColor = 'z';
	char currentSocketColor = 'z';

	// Check for HORIZONTAL groups by running along rows
	for( int socketIndex = 0; socketIndex < sockets.size(); socketIndex++ )
	{
		Socket* nextSocket = sockets.at( socketIndex );
		currentSocketColor = nextSocket->getCurrentJewelType();
		
		int totalSockets = matchingSockets.size();

		// Check if socket color matches current group color
		if( currentSocketColor == currentGroupColor || matchingSockets.empty() )
		{
			// Insert this socket into matching group
			matchingSockets.push_back( nextSocket );
			// Store color for comparisons
			currentGroupColor = currentSocketColor;
		}
		else
		{
			// Jewel-type mismatch, so any existing matching-streak has ended
			colorGroupEnded = true;
			// This socket may be able to start a new streak as a new color group
			addSocketToNextGroup = true;
		}

		// Check if this socket is the last in the row, therefore ending any color group matching streak
		bool endOfRow = ( ( ( socketIndex + 1 ) % gridWidth ) == 0 );
		if( endOfRow )
		{
			// Color group cannot continue past end of 
			colorGroupEnded = true;
			// Next color group will be on next row, so current socket cannot join that group
			addSocketToNextGroup = false;
		}

		// Verify potential group's induction into colorGroups vector
		if( colorGroupEnded )
		{	
			// Reset flag for next group
			colorGroupEnded = false;

			totalSockets = matchingSockets.size();
			// Minimim length must be met to form valid color group
			if( totalSockets >= minimumGroupLength )
			{
				colorGroups.push_back( new ColorGroup( matchingSockets ) );
			}
			// Empty container, ready for next group
			matchingSockets.clear();
		
			// Check whether current socket should be inserted into new potential group (occurs when color-mismatch found on non-row-end)
			if( addSocketToNextGroup )
			{
				// Reset flag for next group
				addSocketToNextGroup = false;
				
				matchingSockets.push_back( nextSocket );
				currentGroupColor = currentSocketColor;
			}
			else
			{
				// Set color back to default non-color to prevent affecting next group's matching
				currentGroupColor = 'z';
			}
		}
	}
	
	// Check for VERTICAL groups in each column
	for( int columnIndex = 0; columnIndex < gridWidth; columnIndex++ )
	{
		// Prevent color groups in separate columns affecting each other
		matchingSockets.clear();
		currentGroupColor = 'z';
		addSocketToNextGroup = true;
		colorGroupEnded = true;

		// For each column there is the same number of socket indices between the top and bottom socket
		int bottomSocketIndexInColumn = columnIndex + ( gridWidth * ( gridWidth - 1 ));
		// For each consecutive column-wise entry, socketIndex increases by gridWidth
		for( int socketIndex = columnIndex; socketIndex <= bottomSocketIndexInColumn; socketIndex += gridWidth )
		{
			Socket* nextSocket = sockets.at( socketIndex );
			currentSocketColor = nextSocket->getCurrentJewelType();
			
			if( currentSocketColor == currentGroupColor || matchingSockets.empty() )
			{
				// Socket matched group (or group had no entries) and so can be added to it
				matchingSockets.push_back( nextSocket );
				currentGroupColor = currentSocketColor;
				colorGroupEnded = false;
				addSocketToNextGroup = false;
			}
			else
			{
				// Mis-match detected, so current group ends
				colorGroupEnded = true;
				addSocketToNextGroup = true;
			}

			if( socketIndex == bottomSocketIndexInColumn )
			{
				colorGroupEnded = true;
				addSocketToNextGroup = false;
			}

			// Once completed, potential groups are validated
			if( colorGroupEnded )
			{
				// Reset flag
				colorGroupEnded = false;

				// Check if minimum length for scoring was reached
				if( matchingSockets.size() >= minimumGroupLength )
				{
					colorGroups.push_back( new ColorGroup( matchingSockets ) );
				}
				// Empty container, ready for next group
				matchingSockets.clear();

				// Socket may be able to form first of next group
				if ( addSocketToNextGroup )
				{
					// Reset flag
					addSocketToNextGroup = false;

					matchingSockets.push_back( nextSocket );
					currentGroupColor = currentSocketColor;
				}				
			}
		}
	}
	
	return colorGroups;
}

int Grid::scoreColorGroups( std::vector<ColorGroup*> &validGroups, int &gameScore)
{
	// If there were no valid groups detected (passed to this function), indicate failure
	int totalValidGroups = validGroups.size();
	if( totalValidGroups == 0 )
	{
		return 0;
	}

	// Scoring Guide:
	// Points rapidly increase with size, with an additional increasing bonus for additional color group scored
	// score = ( baseMultiplier * (groupSize^2) ) + (totalColorGroups-1) * multiGroupBonus)
	int baseMultiplier = 5;
	int multiGroupBonus = 30;
	
	// Track number of groups scored so far in 
	int colorGroupsScored = 0;

	// Calculate scores for current groups
	for( std::vector<ColorGroup*>::iterator groupItr = validGroups.begin(); groupItr != validGroups.end(); groupItr++ )
	{
		colorGroupsScored++;
		int groupSizeSquared = (*groupItr)->getGroupSize() * (*groupItr)->getGroupSize();
		gameScore += baseMultiplier * groupSizeSquared + ( ((colorGroupsScored-1)*(colorGroupsScored-1)) * multiGroupBonus);

		// Remove jewels from scored sockets
		std::vector<Socket*>::iterator socketsBegin = (*groupItr)->getGroupSocketsBeginning();
		std::vector<Socket*>::iterator socketsEnd = (*groupItr)->getGroupSocketsEnd();
		for( std::vector<Socket*>::iterator socketIterator = socketsBegin; socketIterator != socketsEnd; socketIterator++)
		{
			char jewelCol = (*socketIterator)->getCurrentJewelType();
			// Remove currentJewel from socket
			(*socketIterator)->discardJewel();
		}
	}
	validGroups.clear();

	return colorGroupsScored;
}

bool Grid::socketsAreFull( int deltaTime )
{
	bool socketsAreFull = true;
	// Iterate BACKWARDS over all sockets, checking if they have jewels
	// Backwards iteration allows bottom rows to be collapsed first, which seems more logical 
	//   (sockets stolen from then subsequently steal during same loop)
	// Top row is excluded from this
	for( int i = sockets.size()-1; i >= gridWidth; i-- )
	{
		Socket* socket = sockets.at( i );
		if( socket->getCurrentJewel() == NULL )
		{
			// Indicate that at least one empty socket was discovered
			socketsAreFull = false;

			// This socket is missing a jewel and must attempt to steal one from the socket above
			Jewel* stolenJewel = sockets.at( i-gridWidth )->relinquishJewel();
			if( stolenJewel != NULL )
			{
				if( socket->setJewel( stolenJewel ) )
				{
					// Set jewel destination
					socket->setCurrentJewelDestination( socket->getSocketBound() );
				}
			}
		}
	}

	// Top row cannot steal jewels from above and so must generate new jewels
	for( int i = gridWidth - 1; i >= 0; i-- )
	{
		Socket* socket = sockets.at( i );
		if( socket->getCurrentJewel() == NULL )
		{
			// Indicate that at least one empty socket was discovered
			socketsAreFull = false;
	
			// Generate jewel one socket-width higher than this socket
			socket->generateOffsetJewel( 0, -cellHeight );
			// Set the jewel's destination as socket so it will fall
			socket->getCurrentJewel()->setNewDestination( socket->getSocketBound() );
		}
	}

	return true;
}

bool Grid::jewelsAreStatic( int deltaTime )
{
	bool jewelsAreStatic = true;
	// Iterate through sockets, checking if jewelBound locations match the jewel destination
	for( int i = 0; i < sockets.size(); i++ )
	{
		Socket* socket = sockets.at( i );
		Jewel* currentJewel = socket->getCurrentJewel();
		if( currentJewel != NULL )
		{
			if( currentJewel->inTransit() )
			{
				jewelsAreStatic = false;
				socket->moveJewelToDestination( deltaTime );
			}
		}
	}
	return jewelsAreStatic;
}