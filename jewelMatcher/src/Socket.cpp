#include "SDL.h"
#include "../include/Socket.h"

#include <iostream>

Socket::Socket( SDL_Rect newSocketBound ): 
	socketBound( newSocketBound ),
	currentJewel( NULL )
{
}

Socket::~Socket()
{
	// Member pointer currentJewel must be cleaned-up
	delete currentJewel;
}

SDL_Rect Socket::getSocketBound() const 
{
	return socketBound;
}

Jewel* Socket::getCurrentJewel() const
{
	return currentJewel;
}

char Socket::getCurrentJewelType() const
{
	if( currentJewel != NULL )
	{
		return currentJewel->getJewelType();
	}
	// Return non-color value
	return 'a';
}

bool Socket::containsJewel() const
{
	if( currentJewel != NULL )
	{
		return true;
	}
	return false;
}

bool Socket::setJewel( Jewel* newJewel )
{
	if( newJewel != NULL )
	{
		currentJewel = newJewel;
		return true;
	}
	return false;
}

void Socket::generateJewel( SDL_Rect const newJewelBound )
{	
	discardJewel();
	// Generate new jewel for socket to contain
	setJewel( new Jewel( newJewelBound ) );
}

void Socket::generateOffsetJewel( int const x, int const y )
{	
	// Use socket coordinates as starting point to which offset is applied
	SDL_Rect newJewelBound = getSocketBound();
	newJewelBound.x += x;
	newJewelBound.y += y;
	generateJewel( newJewelBound );
}

Jewel* Socket::relinquishJewel()
{
	if( currentJewel != NULL )
	{
		// Cannot give up a jewel which has not arrived
		if( !currentJewel->inTransit() )
		{
			// This socket has a jewel which it now must relinquish
			Jewel* jewel = currentJewel;
			currentJewel = NULL;
			return jewel;
		}
	}
	return NULL;
}

void Socket::discardJewel()
{
	if( currentJewel != NULL )
	{
		// Discarded jewel's object must be destroyed
		delete currentJewel;
		currentJewel = NULL;
	}
}

bool Socket::setCurrentJewelDestination( SDL_Rect const newDestination )
{
	Jewel* jewel = getCurrentJewel();
	if( jewel != NULL )
	{
		jewel->setNewDestination( newDestination );
		return true;
	}
	return false;
}

bool Socket::moveJewelToDestination( int const deltaTime )
{
	return getCurrentJewel()->moveToDestination( deltaTime );
}