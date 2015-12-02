// Handler for trading between different types of resources.
// - Ronan

#ifndef SIMPLEMUDTRADINGHANDLER_H
#define SIMPLEMUDTRADINGHANDLER_H

#include "../BasicLib/BasicLib.h"
#include "../SocketLib/SocketLib.h"
#include "Player.h"
#include "PlayerDatabase.h"
#include "DatabasePointer.h"
#include "SimpleMUDLogs.h"
#include <string>
#include "Attributes.h"

using SocketLib::Telnet;
using SocketLib::Connection;
using std::string;

namespace SimpleMUD
{
	class TradingHandler : public Telnet::handler
	{
		// typedef the class because MSVC6 isn't smart enough to construct
		// a "Telnet::handler" object, yet it will construct a typedef just
		// fine.  *boggle*
		typedef Telnet::handler thandler;
	public:

		// ------------------------------------------------------------------------
		//  Construct the handler with a reference to the connection so that it can
		//  be used later on. Handlers are initialized only once, and cannot change
		//  connections. A flaw in MSVC6 neccessitated the typedeffing of
		//  Telnet::handler to thandler in order to call the base constructor.
		// ------------------------------------------------------------------------
		TradingHandler(Connection<Telnet>& p_conn, player p_player)
			: thandler(p_conn)
		{
			m_player = p_player;
		}

		// ------------------------------------------------------------------------
		//  Handler Functions
		// ------------------------------------------------------------------------
		void Handle(string p_data);
		// Do stuff when you enter the handler.
		void Enter();
		void Leave() {};
		void Hungup()
		{
			USERLOG.Log(m_player->Name() + " hung up in a devil room handler.");
		};
		void Flooded()
		{
			USERLOG.Log(m_player->Name() + " kicked for flooding in a devil room handler.");
		};

	protected:
		player m_player;

		resource TradeDown(resource p_amount);
		ResourceType EvalResouce(ResourceType p_origType, int p_indexChange);

	};  // end class TradingHandler.




}   // end namespace SimpleMUD

#endif
