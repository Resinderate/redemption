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
		// Some sort of function to a handler.
		// Probably need some sort of factory to figure out what type of handler you need.


	};  // end class SpecialRoom.

}   // end namespace SimpleMUD


#endif
