#ifndef COLORGROUP_H
#define COLORGROUP_H

#include <stdlib.h>
#include <vector>

#include "Socket.h"

class ColorGroup
{
		std::vector<Socket*> socketsInGroup;
		SDL_Rect groupBound;
		char groupColor;
	public:
		ColorGroup();
		ColorGroup(std::vector<Socket*> includedSockets);
		
		int getGroupSize();
		char getGroupColor();

		// Recalculates bounding Rect using group's current socket members
		void updateGroupBound();

		std::vector<Socket*>::iterator getGroupSocketsBeginning();
		std::vector<Socket*>::iterator getGroupSocketsEnd();
};

#endif