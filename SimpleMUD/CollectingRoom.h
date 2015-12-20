// Room particular to collecting resources. Makes up the bulk of the rooms in the game.
// Ronan

// This might not be correct?
#ifndef COLLECTINGROOM_H
#define COLLECTINGROOM_H

#include <string>
#include <chrono>

#include "Room.h"

using std::ostream;
using std::istream;
using std::list;


namespace SimpleMUD
{

	// --------------------------------------------------------------------
	//  Class for rooms used for collecting resources.
	// --------------------------------------------------------------------
	class CollectingRoom : public Room
	{
	public:
		CollectingRoom();
		CollectingRoom(string p_name, string p_desc, RoomBaseType p_baseType, BasicLib::vector2 p_coords, 
			ResourceType p_reType, ResourceSize p_reSize, string p_owner, int p_boughtTimes);

		BasicLib::resource Collect();

		inline ResourceType GetResourceType() { return m_resourceType; }
		inline ResourceSize GetResourceSize() { return m_resourceSize; }

		BasicLib::resource CurrentCost();
		inline string& Owner() { return m_owner; }
		inline int& BoughtTimes() { return m_boughtTimes; }


	protected:
		// What sort of resource it is.
		ResourceType m_resourceType;
		// Size of the resource.
		ResourceSize m_resourceSize;
		// Time since last collection.
		std::chrono::system_clock::time_point m_lastCollection;

		// Owner of the land.
		// Not sure how we want to store this just yet, will probably be the player..
		string m_owner;
		int m_boughtTimes;

	};  // end class CollectingRoom.

}   // end namespace SimpleMUD


#endif
