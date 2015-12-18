// Example of a handler.
// Leaderboard handler.
// - Kevin

#ifndef SIMPLEMUDLEADERBOARDHANDLER_H
#define SIMPLEMUDLEADERBOARDHANDLER_H

#include "../SocketLib/SocketLib.h"
#include "Player.h"
#include "Room.h"
#include "PlayerDatabase.h"
#include "DatabasePointer.h"
#include "SimpleMUDLogs.h"
#include <string>

using SocketLib::Telnet;
using SocketLib::Connection;
using std::string;

namespace SimpleMUD
{

	class LeaderboardHandler : public Telnet::handler
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
		LeaderboardHandler(Connection<Telnet>& p_conn, player p_player)
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
			USERLOG.Log(m_player->Name() + " hung up in a leaderboard handler.");
		};
		void Flooded()
		{
			USERLOG.Log(m_player->Name() + " kicked for flooding in a leaderboard handler.");
		};
		void Leaderboard(int p_data);
	protected:
		player m_player;

	};  // end class ExampleHandler.




}   // end namespace SimpleMUD

#endif
