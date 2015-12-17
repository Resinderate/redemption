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
#include "PlaceNameGenerator.h"

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
		static std::shared_ptr<Room>& GetRoom(BasicLib::vector2 p_coords);
		static BasicLib::vector2 ChangeRoom(BasicLib::vector2 p_coords, BasicLib::vector2 p_move);
		static bool RoomExists(BasicLib::vector2 p_coords);

	protected:
		static RoomDatabase m_rooms;		
		static void GenerateNewRoom(BasicLib::vector2 p_coords);

	};  // end class World.

}   // end namespace SimpleMUD


#endif
