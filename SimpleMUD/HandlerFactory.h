// Class which uses factory pattern to give back the appropriate handler based on the type of room.
// - Ronan

// This might not be correct?
#ifndef HANDLERFACTORY_H
#define HANDLERFACTORY_H

#include <string>
#include <memory>

#include "Attributes.h"
#include "../SocketLib/SocketLib.h"
#include "ExampleHandler.h"

namespace SimpleMUD
{
	// --------------------------------------------------------------------
	//  Class for rooms used for collecting resources.
	// --------------------------------------------------------------------
	class HandlerFactory
	{
	public:
		static std::shared_ptr<SocketLib::Telnet::handler> GetHandler(RoomType p_type, SocketLib::Connection<Telnet>& p_conn, player p_player);
	protected:



	};  // end class HandlerFactory.

}   // end namespace SimpleMUD


#endif
