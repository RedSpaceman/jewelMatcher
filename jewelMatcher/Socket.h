#ifndef SOCKET_H
#define SOCKET_H

#include "Jewel.h"

class Socket
{
	private:
		Jewel* currentJewel;
	public:
		Socket::Socket( int x, int y, int w, int h );

		void generateJewel( int x, int y );
		Jewel* relinquishJewel();
};

#endif