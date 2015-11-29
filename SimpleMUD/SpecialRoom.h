// Room for special interactions.
// Ronan

// This might not be correct?
#ifndef SPECIALROOM_H
#define SPECIALROOM_H

#include <string>

#include "Room.h"

namespace SimpleMUD
{


	// --------------------------------------------------------------------
	//  Class for rooms used for collecting resources.
	// --------------------------------------------------------------------
	class SpecialRoom : public Room
	{
	public:
		SpecialRoom();
		SpecialRoom(string p_name, string p_desc, RoomBaseType p_baseType, BasicLib::vector2 p_coords, RoomType p_roomType);


	protected:
		// Probably some sort of enum for the type.
		RoomType m_roomType;

		// Need some way to pick the handler that would be used for each special room.
		// Based on the RoomType, but not sure who wants it / where to create it...


	};  // end class SpecialRoom.

}   // end namespace SimpleMUD


#endif
