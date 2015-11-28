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


	protected:
		// Probably some sort of enum for the type.
		RoomType m_roomType;

		// Some sort of function to a handler.
		// Probably need some sort of factory to figure out what type of handler you need.
		// Could just send a room type to the factory and it figures the rest out.
		// Would be nice to avoid having to mess with dynamic typing in the factory though.

		// Could have some sort of identifier in the class I guess.
		// Use case like: <parse> Interact..
			// Figure out if the room is interactable.. ie. is it a special room.

		// Would almost be better off using composition in that case..


	};  // end class SpecialRoom.

}   // end namespace SimpleMUD


#endif
