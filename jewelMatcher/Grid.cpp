#include "Grid.h"

Grid::Grid(int newGridWidth, int newCentreX, int newCentreY, int newCellWidth, int newCellHeight){
	gridWidth = newGridWidth;
	cellWidth = newCellWidth;
	cellHeight = newCellHeight;

	gridCentreX = newCentreX;
	gridCentreY = newCentreY;

	gridXStart = (int)(gridCentreX-(cellWidth*(gridWidth/2)));
	gridYStart = (int)(gridCentreY-(cellHeight*(gridWidth/2)));
	// Offset grid drawing if odd-numbered grid width
	if (gridWidth%2 != 0 )
	{
		gridXStart -= cellWidth/2;
		gridYStart -= cellHeight/2;
	}

	// Allocate the number of elements required
	sockets.resize(gridWidth * gridWidth);
	generateSockets();
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
		int socketX = getSocketXLocFromIndex( i );
		int socketY = getSocketYLocFromIndex( i );
		sockets.push_back( new Socket( socketX, socketY, cellWidth, cellHeight ) );
	}
}

void Grid::populateSockets()
{
	// Instruct each socket to generate a new jewel
	int totalSockets = gridWidth*gridWidth;
	for( int i = 0; i < totalSockets; i++)
	{
		int x = getColumnForSocketIndex( i );
		int y = getRowForSocketIndex( i );

		sockets.at(i)->generateJewel( x, y );
	}
}

int Grid::getGridWidth(){
	return gridWidth;
}

Socket* Grid::getSocketAtRowColumn( int row, int column )
{
	return sockets.at(convertRowColumnToSocketIndex(row, column));
}

int Grid::convertRowColumnToSocketIndex(int row, int column)
{
	// Socket index starts in top left of grid (0) and counts rightwards, starting at the left for each new row

	// Convert x and y into a sockets index
	// Testing logic (gW=5)
	//  first column, first row(idx 0): 1,1 -> 0+0
	//  fourth column, second row(idx 8): 4,2 -> 3+(1*5)=8
	return (( column - 1 ) + ((row -1 ) * gridWidth ));
}

int Grid::getRowForSocketIndex(int socketIndex)
{
	return socketIndex % gridWidth;
}

int Grid::getColumnForSocketIndex(int socketIndex)
{
	return (int)( socketIndex / gridWidth );
}

int Grid::getSocketXLocFromIndex(int socketIndex)
{
	return gridXStart + ( cellWidth * ( getColumnForSocketIndex(socketIndex) - 1 ));
}

int Grid::getSocketYLocFromIndex(int socketIndex)
{
	return gridYStart + ( cellHeight * ( getRowForSocketIndex(socketIndex) - 1 ));
}