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

	CollectingRoom::CollectingRoom(string p_name, string p_desc, RoomBaseType p_baseType, BasicLib::vector2 p_coords, 
		ResourceType p_reType, ResourceSize p_reSize) : 
		Room(p_name, p_desc, p_baseType, p_coords),
		m_resourceType(p_reType),
		m_resourceSize(p_reSize)
	{
		m_lastCollection = 0;
		m_owner = NULL;
	}

	BasicLib::resource CollectingRoom::Collect()
	{
		// Based on the Type / Size / TimeSinceLastCollect / Owner, give the player some resources.
		BasicLib::resource reward = 10;

		// 10, 20, 30
		reward *= m_resourceSize + 1;

		// No Owner.

		// No time consideration.
		
		// Reset the timer.
		// m_lastCollection = time.now

		return reward;
	}


}   // end namespace SimpleMUD

