#include "ColorGroup.h"

ColorGroup::ColorGroup(std::vector<Socket*> includedSockets)
{
	// Store group's sockets
	socketsInGroup = includedSockets;
	// Set group color
	groupColor = socketsInGroup.at(0)->getCurrentJewelType();
	// Calculate group bound, used for merging L-Shape color groups into a single group
	SDL_Rect firstSocketBound = socketsInGroup.front()->getSocketBound();
	SDL_Rect lastSocketBound =  socketsInGroup.back()->getSocketBound();
	groupBound.x = firstSocketBound.x;
	groupBound.y = firstSocketBound.y;
	groupBound.w = lastSocketBound.x + lastSocketBound.w - groupBound.x;
	groupBound.h = lastSocketBound.y + lastSocketBound.h - groupBound.y;
}