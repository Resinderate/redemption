// World class to encapsulate the functionality of generating rooms, as well as moving around the world.
// Ronan

// This might not be correct?
#ifndef WORLD_H
#define WORLD_H

#include <string>

// I think including the room will be enough, not sure though.
// Might need the specific rooms, if the base room doesn't define all the functions we need as abstract.
#include "Room.h"

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
		

		// Also needs to 

	};  // end class World.

}   // end namespace SimpleMUD


#endif
