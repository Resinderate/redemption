// Implementation of SpecialRoom class.
// Ronan

#include <string>
#include <iostream>

#include "SpecialRoom.h"

using std::ostream;
using std::istream;
using BasicLib::extract;

namespace SimpleMUD
{

	SpecialRoom::SpecialRoom()
	{
		// Set it to something by default.
		// Ideally this is set up by the world, which manages the rooms.
		m_baseType = RoomBaseType::SPECIAL;
		m_roomType = RoomType::TRADING;
	}

	SpecialRoom::SpecialRoom(string p_name, string p_desc, RoomBaseType p_baseType, BasicLib::vector2 p_coords, RoomType p_roomType) :
		Room(p_name, p_desc, p_baseType, p_coords),
		m_roomType(p_roomType)
	{
	}

}   // end namespace SimpleMUD

