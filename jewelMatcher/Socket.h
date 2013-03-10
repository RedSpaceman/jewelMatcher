#ifndef SOCKET_H
#define SOCKET_H

#include "Jewel.h"

class Socket
{
	private:
		SDL_Rect socketBound;
		Jewel* currentJewel;
	public:
		Socket::Socket( SDL_Rect );

		int getX();
		int getY();
		int Socket::getWidth();
		int Socket::getHeight();

		Jewel* getCurrentJewel();
		char getCurrentJewelType();

		void generateJewel( SDL_Rect newJewelBound );
		Jewel* relinquishJewel();

};

#endif