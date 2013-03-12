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

		void Socket::setJewel(Jewel* newJewel);

		SDL_Rect getSocketBound();
		int Socket::getWidth();
		int Socket::getHeight();

		Jewel* getCurrentJewel();
		char getCurrentJewelType();
		
		// Indicates if socket currently contains a jewel
		bool Socket::containsJewel();

		void generateJewel( SDL_Rect newJewelBound );
		Jewel* relinquishJewel();

		// Set currentJewel to NULL, discarding the socket's contents
		void Socket::discardJewel();

};

#endif