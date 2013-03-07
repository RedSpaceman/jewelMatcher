#include "SDL.h"
#include "Socket.h"

Socket::Socket(int x, int y, int w, int h)
{
	currentJewel = NULL;
}

// When called by another socket, this socket must hand-over its jewel
Jewel* Socket::relinquishJewel()
{
	if( currentJewel != NULL )
	{
		// This socket has a jewel which it now must relinquish
		Jewel* jewel = currentJewel;
		currentJewel = NULL;
		return jewel;
	}
	else
	{
		return NULL;
	}	
}

void Socket::generateJewel( int x, int y )
{	
	currentJewel = new Jewel( x, y );
}
