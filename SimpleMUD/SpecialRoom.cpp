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
		m_roomType = RoomType::TRADING;
	}

}   // end namespace SimpleMUD

