#ifndef COLORGROUP_H
#define COLORGROUP_H

#include <stdlib.h>
#include <vector>

#include "Socket.h"

class ColorGroup
{
	private:
		std::vector<Socket*> socketsInGroup;
		char groupColor;
	public:
		ColorGroup();
		ColorGroup(std::vector<Socket*> includedSockets);
		
		int getGroupSize();
		char getGroupColor();

		std::vector<Socket*>::iterator getGroupSocketsBeginning();
		std::vector<Socket*>::iterator getGroupSocketsEnd();
};

#endif