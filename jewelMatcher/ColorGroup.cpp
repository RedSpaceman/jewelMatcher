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

bool ColorGroup::addSocketToGroup(Socket* socket)
{
	// Only add new socket if its a color match
	if( groupColor == 'z' )
	{
		// This socket is first in group, so set group color
		groupColor = socket->getCurrentJewelType();
	}

	if( groupColor ==  socket->getCurrentJewelType() )
	{
		// Color was a match
		socketsInGroup.push_back(socket);
		updateGroupBound();
		return true;
	}
	return false;
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

std::vector<Socket*>::iterator ColorGroup::getGroupSocketsBeginning()
{
	return socketsInGroup.begin();
}

std::vector<Socket*>::iterator ColorGroup::getGroupSocketsEnd()
{
	return socketsInGroup.end();
}