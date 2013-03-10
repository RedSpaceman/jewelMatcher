#include "SDL.h"
#include "Socket.h"

#include <iostream>

Socket::Socket(SDL_Rect newSocketBound)
{
	socketBound = newSocketBound;
}

SDL_Rect Socket::getSocketBound()
{
	return socketBound;
}

int Socket::getWidth()
{
	return socketBound.w;
}

int Socket::getHeight()
{
	return socketBound.h;
}

Jewel* Socket::getCurrentJewel()
{
	return currentJewel;
}

char Socket::getCurrentJewelType()
{
	return currentJewel->getJewelType();
}

void Socket::generateJewel( SDL_Rect newJewelBound )
{	
	currentJewel = new Jewel( newJewelBound );
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
