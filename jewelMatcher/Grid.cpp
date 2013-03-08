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
		int socketX = getSocketXCentreFromIndex( i );
		int socketY = getSocketYCentreFromIndex( i );
		sockets.push_back( new Socket( socketX, socketY, cellWidth, cellHeight ) );
	}
}

void Grid::populateSockets()
{
	// Instruct each socket to generate a new jewel
	int totalSockets = gridWidth*gridWidth;
	for( int i = 0; i < totalSockets; i++)
	{
		int jewelX = getSocketXCentreFromIndex( i );
		int jewelY = getSocketYCentreFromIndex( i );
		sockets.at(i)->generateJewel( jewelX, jewelY );
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

int Grid::getSocketXCentreFromIndex(int socketIndex)
{	
	int temp = ( getColumnForSocketIndex(socketIndex) - 1 );
	return gridXStart + (cellWidth / 2 ) + ( cellWidth *  temp);
}

int Grid::getSocketYCentreFromIndex(int socketIndex)
{
	return gridYStart + (cellHeight / 2 ) + ( cellHeight * ( getRowForSocketIndex(socketIndex) - 1 ) ); 
}

std::vector<Socket*>::iterator Grid::getSocketsBeginning()
{
	return sockets.begin();
}

std::vector<Socket*>::iterator Grid::getSocketsEnd()
{
	return sockets.end();
}