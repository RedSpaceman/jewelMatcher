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

		// Returns true if a socket matches group's colour type (which is set if not already) and pushes socket into vector
		bool ColorGroup::addSocketToGroup(Socket* socket);
		// Recalculates bounding Rect using group's current socket members
		void ColorGroup::updateGroupBound();

		int ColorGroup::getGroupSize();

		std::vector<Socket*>::iterator ColorGroup::getGroupSocketsBeginning();
		std::vector<Socket*>::iterator ColorGroup::getGroupSocketsEnd();
};

#endif