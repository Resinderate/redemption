// Room particular to collecting resources. Makes up the bulk of the rooms in the game.
// Ronan

// This might not be correct?
#ifndef SIMPLEMUDCOLLECTINGROOM_H
#define SIMPLEMUDCOLLECTINGROOM_H

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


	protected:
		// What sort of resource it is.
		// Size of the resource.
		// Time since last collection.
		// Owner of the land.

		// Someone buying the land is probably handled externally.
		// Logic at least, owner can then be assigned.
		

	};  // end class CollectingRoom.

}   // end namespace SimpleMUD


#endif
