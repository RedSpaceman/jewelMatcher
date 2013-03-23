#include "../include/ColorGroup.h"

ColorGroup::ColorGroup(): 
	groupColor('z')
{
}

ColorGroup::ColorGroup(std::vector<Socket*> includedSockets):
	socketsInGroup( includedSockets ),							// Store group's sockets
	groupColor( socketsInGroup.at(0)->getCurrentJewelType() )	// Set group color
{
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