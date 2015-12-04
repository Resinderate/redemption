// Room for special interactions.
// Ronan

// This might not be correct?
#ifndef SPECIALROOM_H
#define SPECIALROOM_H

#include <string>
#include <memory>

#include "Room.h"
#include "../SocketLib/SocketLib.h"

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

		inline RoomType GetRoomType() { return m_roomType; }

	protected:
		// Probably some sort of enum for the type.
		RoomType m_roomType;

	};  // end class SpecialRoom.

}   // end namespace SimpleMUD


#endif
