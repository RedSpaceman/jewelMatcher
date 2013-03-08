#ifndef SOCKET_H
#define SOCKET_H

#include "Jewel.h"

class Socket
{
	private:
		int socketX;
		int socketY;
		int width;
		int height;
		Jewel* currentJewel;
	public:
		Socket::Socket( int x, int y, int w, int h );

		int getX();
		int getY();
		int Socket::getWidth();
		int Socket::getHeight();

		Jewel* getCurrentJewel();
		char getCurrentJewelType();

		void generateJewel( int x, int y );
		Jewel* relinquishJewel();

};

#endif