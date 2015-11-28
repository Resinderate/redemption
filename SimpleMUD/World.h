// World class to encapsulate the functionality of generating rooms, as well as moving around the world.
// Ronan

// This might not be correct?
#ifndef WORLD_H
#define WORLD_H

#include <string>

// I think including the room will be enough, not sure though.
// Might need the specific rooms, if the base room doesn't define all the functions we need as abstract.
#include "Room.h"
#include "RoomDatabase.h"

using std::ostream;
using std::istream;
using std::list;

namespace SimpleMUD
{
	// --------------------------------------------------------------------
	//  Class for rooms used for collecting resources.
	// --------------------------------------------------------------------
	class World
	{
	public:
		World();


	protected:
		// The database will store all of out rooms I think.
			// Could be stored with key as the Coords.
		// So it has all the rooms.
		RoomDatabase m_rooms;
		
		// Needs to take commands, saying where to go.
			// Takes in the move. Along with the room, or the current coords.
			// Then tries to find the next room. 
			// If there is no room of that type then it creates a new room using some algo.

		// Function to create new rooms.
			// Might be sub functions for the details.

		// How does this work with an almost blank room?
		// Loading doesn't really work.
		

	};  // end class World.

}   // end namespace SimpleMUD


#endif
