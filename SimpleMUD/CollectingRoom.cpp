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
		m_lastCollection = std::chrono::system_clock::now() - std::chrono::duration<int>(120);
		m_owner = NULL;
	}

	CollectingRoom::CollectingRoom(string p_name, string p_desc, RoomBaseType p_baseType, BasicLib::vector2 p_coords, 
		ResourceType p_reType, ResourceSize p_reSize) : 
		Room(p_name, p_desc, p_baseType, p_coords),
		m_resourceType(p_reType),
		m_resourceSize(p_reSize)
	{
		m_lastCollection = std::chrono::system_clock::now() - std::chrono::duration<int>(120);
		m_owner = NULL;
	}

	BasicLib::resource CollectingRoom::Collect()
	{
		// Based on the Type / Size / TimeSinceLastCollect / Owner, give the player some resources.
		BasicLib::resource reward = 10;

		// Size of resource consideration.
		int sizeMult = 0;
		if (m_resourceSize == ResourceSize::SMALL)
			sizeMult = 1;
		else if (m_resourceSize == ResourceSize::MEDIUM)
			sizeMult = 2;
		else if (m_resourceSize == ResourceSize::LARGE)
			sizeMult = 5;

		reward *= sizeMult;

		// Time consideration.
		auto diff = std::chrono::system_clock::now() - m_lastCollection;
		int secs = std::chrono::duration_cast<std::chrono::seconds>(diff).count();

		if (secs < 10)
			secs = 10;
		else if (secs > 100)
			secs = 100;

		// Multiplier from 0.1 to 1.0
		auto timeMult = secs / 100.f;

		reward *= timeMult;
		
		// Reset the timer.
		m_lastCollection = std::chrono::system_clock::now();

		return reward;
	}


}   // end namespace SimpleMUD

