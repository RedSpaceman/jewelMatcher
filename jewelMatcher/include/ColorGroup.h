#ifndef COLORGROUP_H
#define COLORGROUP_H

#include <stdlib.h>
#include <vector>

#include "../include/Socket.h"

// Used to point to a series of sockets whose jewels form a matching set and will be removed and scored together
class ColorGroup
{
	private:
		std::vector<Socket*> socketsInGroup;
		char groupColor;
	public:
		/* Constructor */
		ColorGroup();
		ColorGroup(std::vector<Socket*> includedSockets);
		
		/* Getter Functions */
		int getGroupSize();
		char getGroupColor();

		std::vector<Socket*>::iterator getGroupSocketsBeginning();
		std::vector<Socket*>::iterator getGroupSocketsEnd();
};

#endif