#include "ColorGroup.h"

ColorGroup::ColorGroup(){
	groupColor = 'z';
	updateGroupBound();
}

ColorGroup::ColorGroup(std::vector<Socket*> includedSockets)
{
	// Store group's sockets
	socketsInGroup = includedSockets;
	// Set group color
	groupColor = socketsInGroup.at(0)->getCurrentJewelType();
}

void ColorGroup::updateGroupBound()
{
	if( socketsInGroup.size() > 0 )
	{
		// Calculate group bound, used for merging L-Shape color groups into a single group
		SDL_Rect firstSocketBound = socketsInGroup.front()->getSocketBound();
		SDL_Rect lastSocketBound =  socketsInGroup.back()->getSocketBound();
		groupBound.x = firstSocketBound.x;
		groupBound.y = firstSocketBound.y;
		groupBound.w = lastSocketBound.x + lastSocketBound.w - groupBound.x;
		groupBound.h = lastSocketBound.y + lastSocketBound.h - groupBound.y;
	}
	else
	{
		groupBound.x = 0;
		groupBound.y = 0;
		groupBound.w = 0;
		groupBound.h = 0;
	}
}

int ColorGroup::getGroupSize()
{
	return socketsInGroup.size();
}

char ColorGroup::getGroupColor()
{
	return groupColor;
}

std::vector<Socket*>::iterator ColorGroup::getGroupSocketsBeginning()
{
	return socketsInGroup.begin();
}

std::vector<Socket*>::iterator ColorGroup::getGroupSocketsEnd()
{
	return socketsInGroup.end();
}