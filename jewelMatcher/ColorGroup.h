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
		ColorGroup::ColorGroup();
		ColorGroup::ColorGroup(std::vector<Socket*> includedSockets);

		// Recalculates bounding Rect using group's current socket members
		void ColorGroup::updateGroupBound();

		int ColorGroup::getGroupSize();
		char ColorGroup::getGroupColor();

		std::vector<Socket*>::iterator ColorGroup::getGroupSocketsBeginning();
		std::vector<Socket*>::iterator ColorGroup::getGroupSocketsEnd();
};

#endif