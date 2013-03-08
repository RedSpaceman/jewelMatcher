#include "SDL.h"
#include "Socket.h"

#include <iostream>

Socket::Socket(int x, int y, int w, int h)
{
	socketX = x;
	socketY = y;
	width = w;
	height = h;
}

int Socket::getX()
{
	return socketX;
}

int Socket::getY()
{
	return socketY;
}

int Socket::getWidth()
{
	return width;
}

int Socket::getHeight()
{
	return height;
}

Jewel* Socket::getCurrentJewel()
{
	return currentJewel;
}

char Socket::getCurrentJewelType()
{
	return currentJewel->getJewelType();
}

void Socket::generateJewel( int jewelX, int jewelY )
{	
	currentJewel = new Jewel( jewelX, jewelY );
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
