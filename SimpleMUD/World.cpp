// Implementation of World class.
// Ronan

#include <string>
#include <iostream>

#include "World.h"

using std::ostream;
using std::istream;
using BasicLib::extract;

namespace SimpleMUD
{

	World::World()
	{
		
	}

	Room& World::GetRoom(BasicLib::vector2 p_coords)
	{
		// Do a while loop, while the room doesn't exist or whatever.
		if (m_rooms.RoomExists(p_coords))
		{
			return m_rooms.GetRoom(p_coords);
		}
		else
		{
			// Maybe generate the new room anyway?
		}
	}

	BasicLib::vector2 World::ChangeRoom(BasicLib::vector2 p_coords, BasicLib::vector2 p_move)
	{
		//BasicLib::vector2 newLocation = p_coords + p_move;
		return BasicLib::vector2();
	}

}   // end namespace SimpleMUD

