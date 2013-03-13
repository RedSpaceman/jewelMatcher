#include "SDL.h"
#include "Socket.h"

#include <iostream>

Socket::Socket(SDL_Rect newSocketBound)
{
	socketBound = newSocketBound;
}

bool Socket::setJewel(Jewel* newJewel)
{
	if( newJewel != NULL )
	{
		currentJewel = newJewel;
		return true;
	}
	return false;
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
	if( currentJewel != NULL )
	{
		return currentJewel->getJewelType();
	}
	// Return non-color value
	return 'a';
}

bool Socket::containsJewel()
{
	if( currentJewel != NULL )
	{
		return true;
	}
	return false;
}

void Socket::generateJewel( SDL_Rect newJewelBound )
{	
	currentJewel = new Jewel( newJewelBound );
}

void Socket::generateOffsetJewel( int x, int y )
{	
	// Use socket coordinates as starting point to which offset is applied
	SDL_Rect newJewelBound = getSocketBound();
	newJewelBound.x += x;
	newJewelBound.y += y;
	generateJewel( newJewelBound );
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

void Socket::discardJewel()
{
	currentJewel = NULL;
}

bool Socket::setCurrentJewelDestination( SDL_Rect newDestination )
{
	if( getCurrentJewel() != NULL )
	{
		getCurrentJewel()->setNewDestination( newDestination );
		return true;
	}
	return false;
}

bool Socket::moveJewelToDestination( int deltaTime )
{
	return getCurrentJewel()->moveToDestination( deltaTime );
}