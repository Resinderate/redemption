// Room particular to collecting resources. Makes up the bulk of the rooms in the game.
// Ronan

// This might not be correct?
#ifndef COLLECTINGROOM_H
#define COLLECTINGROOM_H

#include <string>

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
			ResourceType p_reType, ResourceSize p_reSize);

		BasicLib::resource Collect();

		inline ResourceType GetResourceType() { return m_resourceType; }
		inline ResourceSize GetResourceSize() { return m_resourceSize; }



	protected:
		// What sort of resource it is.
		ResourceType m_resourceType;
		// Size of the resource.
		ResourceSize m_resourceSize;
		// Time since last collection.
		BasicLib::sint64 m_lastCollection;

		// Owner of the land.
		// Not sure how we want to store this just yet, will probably be the player..
		player m_owner;

	};  // end class CollectingRoom.

}   // end namespace SimpleMUD


#endif
