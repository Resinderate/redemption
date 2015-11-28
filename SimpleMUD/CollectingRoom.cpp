// Implementation of CollectingRoom class.
// Ronan

#include <string>
#include <iostream>

#include "CollectingRoom.h"

using std::ostream;
using std::istream;
using BasicLib::extract;

namespace SimpleMUD
{

	CollectingRoom::CollectingRoom()
	{
		m_baseType = RoomBaseType::COLLECTING;
		m_resourceType = ResourceType::WOOD;
		m_resourceSize = ResourceSize::SMALL;
		m_lastCollection = 0;
		m_owner = NULL;
	}


}   // end namespace SimpleMUD

