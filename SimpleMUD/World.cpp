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
		// While there is not room generated.
		// Shouldn't really be able to get to a room that hasn't been generated, but as a fail safe.
		if (!m_rooms.RoomExists(p_coords))
		{
			GenerateNewRoom(p_coords);
		}
		return m_rooms.GetRoom(p_coords);
	}

	BasicLib::vector2 World::ChangeRoom(BasicLib::vector2 p_coords, BasicLib::vector2 p_move)
	{
		BasicLib::vector2 newLocation = BasicLib::vector2(p_coords.x + p_move.x, p_coords.y + p_move.y);
		
		if (!m_rooms.RoomExists(newLocation))
		{
			GenerateNewRoom(newLocation);
		}

		return newLocation;
	}

	void World::GenerateNewRoom(BasicLib::vector2 p_coords)
	{
		
		// Based on some params such as how far away from the centre the coords are:
			// Generate a new room, and add it to the database.
		//m_rooms.AddRoom(p_coords, SpecialRoom());
		m_rooms.AddRoom(p_coords, CollectingRoom());

	}

}   // end namespace SimpleMUD

