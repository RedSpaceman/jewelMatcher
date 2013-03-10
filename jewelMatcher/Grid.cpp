#include "Grid.h"

Grid::Grid(int newGridWidth, int newCentreX, int newCentreY, int newCellWidth, int newCellHeight){
	gridWidth = newGridWidth;
	cellWidth = newCellWidth;
	cellHeight = newCellHeight;

	// If gridWidth is odd, gridWidth%2 allows additional half-width shift to centre central socket
	gridBoundary.x = (int)(newCentreX-(cellWidth*(gridWidth*0.5))) - (int)((gridWidth%2) * (cellWidth*0.5));
	gridBoundary.y = (int)(newCentreY-(cellHeight*(gridWidth*0.5))) - (int)((gridWidth%2) * (cellHeight*0.5));
	gridBoundary.w = gridWidth * cellWidth;
	gridBoundary.h = gridWidth * cellHeight;

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
	return gridBoundary.x + (cellWidth / 2 ) + ( cellWidth *  temp);
}

int Grid::getSocketYCentreFromIndex(int socketIndex)
{
	return gridBoundary.y + (cellHeight / 2 ) + ( cellHeight * ( getRowForSocketIndex(socketIndex) - 1 ) ); 
}

SDL_Rect Grid::calcSocketBoundaryFromIndex(int socketIndex)
{
	SDL_Rect socketBoundary;
	socketBoundary.x = gridBoundary.x + ( cellWidth * ( getColumnForSocketIndex(socketIndex) - 1 ) );
	socketBoundary.y = gridBoundary.y + ( cellHeight * ( getRowForSocketIndex(socketIndex) - 1 ) ); 
	socketBoundary.w = cellWidth;
	socketBoundary.h = cellHeight;

	return socketBoundary;
}

std::vector<Socket*>::iterator Grid::getSocketsBeginning()
{
	return sockets.begin();
}

std::vector<Socket*>::iterator Grid::getSocketsEnd()
{
	return sockets.end();
}

