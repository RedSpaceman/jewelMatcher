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
		ColorGroup::ColorGroup(std::vector<Socket*> includedSockets);
};

#endif