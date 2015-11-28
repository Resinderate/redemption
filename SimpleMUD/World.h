// World class to encapsulate the functionality of generating rooms, as well as moving around the world.
// Ronan

// This might not be correct?
#ifndef WORLD_H
#define WORLD_H

#include <string>

// I think including the room will be enough, not sure though.
// Might need the specific rooms, if the base room doesn't define all the functions we need as abstract.
#include "SpecialRoom.h"
#include "CollectingRoom.h"
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
		
		// Should probably be a safe part here, where if you are 0, 0. It makes a new room before sending it back?
		Room& GetRoom(BasicLib::vector2 p_coords);

		// Needs to take commands, saying where to go.
			// Takes in current coords, as well as a move vector showing direction
			
			// Then tries to find the next room. 
			// If there is no room of that type then it creates a new room using some algo.
		// Gives back the new coordinates as a confirmation of the move.
		// Those coords can then be fed back into world to get the room.
		BasicLib::vector2 ChangeRoom(BasicLib::vector2 p_coords, BasicLib::vector2 p_move);

		// Function to create new rooms.
			// Based on the coords make a new room.
			// Used by ChangeRoom to add a new room to the RoomDatabase.
		void GenerateNewRoom(BasicLib::vector2 p_coords);

		// How does this work with an almost blank room?
		// Loading doesn't really work.
			// Will just work it out.
		

	};  // end class World.

}   // end namespace SimpleMUD


#endif
