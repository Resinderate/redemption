// MUD Programming
// Ron Penton
// (C)2003
// Game.cpp - This class is the game handler for SimpleMUD.
// 
// 

#include "Logon.h"
#include "Game.h"
#include "PlayerDatabase.h"
#include "LeaderboardHandler.h"
#include "LanguageFilter.h"
#include "../SocketLib/ConnectionManager.h"
#include "../BasicLib/BasicLib.h"
#include "../BasicLib/BasicLibString.h"

using namespace SocketLib;
using namespace BasicLib;
using std::string;

#define titledName "[" + GetTitleString(p.GetPlayerTitle()) + "] " + p.Name()
namespace SimpleMUD
{


// declare the static instance of the timer
BasicLib::Timer Game::s_timer;
bool Game::s_running = false;

CommandDictionary Game::m_dictionary;
bool Game::m_dictInitialized = false;

// ------------------------------------------------------------------------
//  This handles incomming commands. Anything passed into this function
//  is assumed to be a complete command from a client.
// ------------------------------------------------------------------------
void Game::Handle(string p_data)
{
	Player& p = *m_player;

	// Reset the latest activity
	p.LatestActivity() = std::chrono::system_clock::now();

	// check if the player wants to repeat a command
	if (p_data == "/")
	{
		p_data = m_lastcommand;
	}
	else
	{
		// if not, record the command.
		m_lastcommand = p_data;
	}

	// get the first word and lowercase it.
	string firstword = BasicLib::LowerCase(ParseWord(p_data, 0));



	// ------------------------------------------------------------------------
	//  REGULAR access commands
	//	@author: Kevin Duffy
	// ------------------------------------------------------------------------
	// message sent to current Tile
	if (firstword == "say")
	{
			if (!p.Mute())
			{
		string text = RemoveWord(p_data, 0);
		string unfiltered = text;
		if (LanguageFilter::Filter(text))
			BADLANGLOG.Log(p.Name() + " message filtered: " + unfiltered);

		SendRoom(magenta + bold + titledName + " -> Room: " + dim + text, *p.CurrentRoom());
		return;
	}
			else
				p.SendString(red + bold + "You have been muted");
			return;
		}

	// message sent to current and adjacent Tiles
	if (firstword == "shout")
	{
		if (!p.Mute())
		{
			string text = RemoveWord(p_data, 0);
			string unfiltered = text;
			if (LanguageFilter::Filter(text))
				BADLANGLOG.Log(p.Name() + " message filtered: " + unfiltered);

			list<vector2>& rlist = p.AdjacentRooms();
			list<vector2>::iterator ritr = rlist.begin();
			while (ritr != rlist.end())
			{
				vector2& v = *ritr;
				SendRoom(cyan + bold + titledName + " -> Local:  " + dim + text, *World::GetRoom(v));
				++ritr;

			}
			return;
		}
		else
			p.SendString(red + bold + "You have been muted");
		return;
	}

	// message sent to all corporation members
	if (firstword == "corp")
	{
			if (!p.Mute())
			{
		if (p.CompName() == CORPNONE)
		{
			p.SendString(red + "You are not in a corporation!");
			return;
		}
		string text = RemoveWord(p_data, 0);
		string unfiltered = text;
		if (LanguageFilter::Filter(text))
			BADLANGLOG.Log(p.Name() + " message filtered: " + unfiltered);
		auto members = PlayerDatabase::CorpMembers(p.CorpName());
		for (auto member : members)
		{
			member->SendString(yellow + bold + titledName + " -> Corporation: " + white + text);
		}
			}
			else
				p.SendString(red + bold + "You have been muted");
		return;		
	}

	// message sent to whole server
	if (firstword == "global")
	{
			if (!p.Mute())
			{
		string text = RemoveWord(p_data, 0);
		string unfiltered = text;
		if (LanguageFilter::Filter(text))
			BADLANGLOG.Log(p.Name() + " message filtered: " + unfiltered);
		SendGame(blue + bold + titledName + " -> Global: " + white + text);
			}
			else
				p.SendString(red + bold + "You have been muted");
		return;
	}
	//Private message
	if (firstword == "whisper")
	{
			if (!p.Mute())
			{
		// get the players name
		string name = ParseWord(p_data, 1);
		string message = RemoveWord(RemoveWord(p_data, 0), 0);
		string unfiltered = message;
		if (LanguageFilter::Filter(message))
			BADLANGLOG.Log(p.Name() + " message filtered: " + unfiltered);

		if (LowerCase(name) == LowerCase(p.Name()))
		{
			p.SendString(red + "Whispering to yourself may result in being sent to an institution...");
			return;
		}

		if (message == "")
		{
			p.SendString(red + "Did not detect a message!");
			return;
		}

		Whisper(message, name);
			}
			else
				p.SendString(red + bold + "You have been muted");
		return;
	}

	//display all available commands
	if (firstword == "help")
	{
		p.SendString(PrintHelp(p.Rank()));
		return;
	}

	//display all the players shortcuts
	if (firstword == "shortcuts")
	{
		p.SendString(PrintShortcuts());
		return;
	}

	//leave the game
	if (firstword == "exit" || firstword == "quit")
	{
		m_connection->Close();
		LogoutMessage(titledName + " has left the game.");
		return;
	}

	//Check Player stats Tool levels resources etc.
	if (firstword == "stats")
	{
		p.SendString(PrintStats());
		return;
	}

	//Check current time and Server Lifetime
	if (firstword == "time")
	{
		p.SendString(bold + cyan +
			"The current system time is: " + BasicLib::TimeStamp() +
			" on " + BasicLib::DateStamp() +
			"\r\nThe system has been up for: "
			+ GetTimer().GetString() + ".");
		return;
	}

	//display online players/ All players if followed by "all"
	if (firstword == "who")
	{
		p.SendString(WhoList(BasicLib::LowerCase(ParseWord(p_data, 1))));
		return;
	}

	//Description of the current tile
	if (firstword == "look")
	{
		p.SendString(PrintRoom(p.CurrentRoom()));
		return;
	}

	if (firstword == "north")
	{
		Move(NORTH);
		return;
	}
	if (firstword == "east")
	{
		Move(EAST);
		return;
	}
	if (firstword == "south")
	{
		Move(SOUTH);
		return;
	}
	if (firstword == "west")
	{
		Move(WEST);
		return;
	}

	//Display players titles
	if (firstword == "titles")
    {
		string temp = "Available Titles: ";
		auto itr = p.Titles().begin();
		for (itr; itr != p.Titles().end(); ++itr)
		{
			PlayerTitle t = *itr;
			temp += GetTitleString(t) + ", ";
		}

			temp.erase(temp.size() - 2, 2);
			temp += "\r\n";

			p.SendString(white + bold + temp);
            return;
        }

	//Change players title
	if (firstword == "change")
	{
		//Syntax (change <title_name>)

		string secondword = ParseWord(p_data, 1);
		bool changed = p.SetTitle(secondword);
		if (changed)
			p.SendString(green + bold + "Title changed to [" + secondword + "]");
		else
			p.SendString(red + bold + "The title '" + secondword + "' is not abailable" + reset);
		return;
	}

	//Rebind a command to a shortcut key
	if (firstword == "rebind")
	{
		p.Conn()->AddHandler(new PlayerDictHandler(*p.Conn(), p.ID()));
		return;
	}

	//If in a resource room collect whatever resource is there
	if (firstword == "collect")
	{
		// Add in the function for collecting from this room.
		Collect();
		return;
	}

	//give a player a certain amount of a resource
	if (firstword == "trade")
	{
		//Syntax (trade <player_name> <amount> <resource_type>)
		string secondword = ParseWord(p_data, 1);
		if (secondword == "")
		{
			p.SendString(red + "Did not detect a player name!");
			return;
		}

		auto target = PlayerDatabase::findfull(secondword);
		if (target == PlayerDatabase::end())
		{
			p.SendString(red + "Could not find a player of that name!");
			return;
		}

		string thirdword = ParseWord(p_data, 2);
		if (thirdword == "")
		{
			p.SendString(red + "Did not detect an amount!");
			return;
		}
		// Could be an invalid input. Can throw an exception.
		int amount = 0;
		try
		{
			amount = std::stoi(thirdword);
		}
		catch (...)
		{
			p.SendString(red + "Did not detect a valid amount!");
			return;
		}

		string fourthword = ParseWord(p_data, 3);
		if (fourthword == "")
		{
			p.SendString(red + "Did not detect a resource type!");
			return;
		}

		// Find what which resource it is.
		string fourthlower = LowerCase(fourthword);

		int resourceIndex = -1;
		for (int i = 0; i < NumResourceType; i++)
		{
			if (LowerCase(ResourceTypeStrings[i]) == fourthlower)
			{
				resourceIndex = i;
				break;
			}
		}
		if (resourceIndex == -1)
		{
			p.SendString(red + "Did not detect a valid resource type!");
			return;
		}
		ResourceType type = static_cast<ResourceType>(resourceIndex);

		// Check if the player has enough to give.
		if (p.GetResources()[type] < amount)
		{
			p.SendString(red + "Do not have enough of that resource to trade!");
			return;
		}

		// Do the trade.
		p.GetResources()[type] -= amount;
		target->GetResources()[type] += amount;

		// Send messages.
		p.SendString(green + "Successfully sent " + target->Name() + " " + std::to_string(amount) + " " + fourthlower);
		if (target->LoggedIn())
			target->SendString(green + p.Name() + " has sent you " + std::to_string(amount) + " " + fourthlower);

		return;
	}

	//complex Special room command Prompts a dialogue for the player to engage with (Devil Room, Corp Room, Assasin Room)
	if (firstword == "interact")
	{
		// Ronan

		// If the room is of the correct type.
		if (p.CurrentRoom()->GetBaseType() == RoomBaseType::SPECIAL)
		{
			SpecialRoom* sRoom = dynamic_cast<SpecialRoom*>(p.CurrentRoom().get());
			RoomType type = sRoom->GetRoomType();

			// Abandoned the Handler Factory.
			if (type == RoomType::TRADING)
				p.Conn()->AddHandler(new TradingHandler(*p.Conn(), p.ID()));
			else if (type == RoomType::DEVIL)
				p.Conn()->AddHandler(new DevilHandler(*p.Conn(), p.ID()));
			else if (type == RoomType::WORKSHOP)
				p.Conn()->AddHandler(new WorkshopHandler(*p.Conn(), p.ID()));
			else if (type == RoomType::CORP)
				p.Conn()->AddHandler(new CorpHandler(*p.Conn(), p.ID()));
			else if (type == RoomType::ASSASSIN)
				p.Conn()->AddHandler(new AssassinHandler(*p.Conn(), p.ID()));
			return;
		}
		else
		{
			p.SendString(red + "Nothing to interact with in this room!");
			return;
		}
		
	}

	//Invite a player to your corporation as long as you are the leader/ have permissions
	if (firstword == "invite" /*&& check permissions / is part of corp */)
    {
		//Syntax (invite <player_name>)

		// Check if the player is in a corporation and is the leader.
		if (p.CorpName() != CORPNONE && p.CorpLeader())
		{
			// Can invite someone by adding a corp to that player.
			string second = ParseWord(p_data, 1);
			if (second == "")
			{
				p.SendString(red + "Could not detect a player to invite.");
				return;
			}

			// Find the player.
			// See if they are online.
			auto target = PlayerDatabase::findloggedin(second);
			if (target == PlayerDatabase::end())
			{
				p.SendString(red + "Could not detect a player of that name online!");
				return;
			}
			// Check if they already have a corporation.
			if (target->CorpName() != CORPNONE)
			{
				p.SendString(red + "That player is already in a corporation!");
				return;
			}
			// Change their corporation to yours.
			target->CorpName() = p.CorpName();

			p.SendString(green + second + " added to your corporation!");
		}
		else
		{
			p.SendString(red + "You cannot invite a player to your coporation!");
		}
		return;
	}

	//leave current corporation
	if (firstword == "leavecorp")
	{
		if (p.CorpName() == CORPNONE)
		{
			p.SendString(red + "You are not in a corporation to leave!");
			return;
		}
		else
		{
			p.CorpName() = CORPNONE;
			// If they were the leader then give it to someone else.
			
			if (p.CorpLeader())
			{
				auto members = PlayerDatabase::CorpMembers(p.CorpName());
				for (auto& mem : members)
				{
					if (mem->Name() != p.Name())
					{
						mem->CorpLeader() = true;
						break;
					}
				}
				
				p.CorpLeader() = false;
			}
			
			
			p.SendString(green + "Left corporation!");
			return;
		}
	}

	//check certain leaderboard (Player's Corporation/World Rank, Resources Gathered, Corporation Souls Redeemed)
	if (firstword == "leaderboards")
	{
		p.Conn()->AddHandler(new LeaderboardHandler(*p.Conn(), p.ID()));
		return;
	}

	if (firstword == "report")
	{
		string player = ParseWord(p_data, 1);
		if (player == "")
		{
			p.SendString(red + "Did not detect player to report!");
			return;
		}
		string reason = RemoveWord(RemoveWord(p_data, 0), 0);
		if (reason == "")
		{
			p.SendString(red + "Did not detect reason for report!");
			return;
		}
		
		REPORTLOG.Log(m_player->Name() + " reported " + player + ". Reason: " + reason);
		p.SendString("Player, " + player + ", reported! Reason: " + reason);
		return;
	}

	if (firstword == "warp")
	{
		string sX = ParseWord(p_data, 1);
		string sY = ParseWord(p_data, 2);

		if (sX == "" || sY == "")
		{
			p.SendString(red + "Did not detect X and Y coords.");
			return;
		}
		int x = std::stoi(sX);
		int y = std::stoi(sY);

		if (!World::RoomExists(vector2(x, y)))
		{
			p.SendString(red + "That place has not been explored yet!");
			return;
		}

		std::shared_ptr<Room> prev = p.CurrentRoom();
		// Interface not meant to be used this way..
		p.Coords() = World::ChangeRoom(vector2(x, y), vector2(0, 0));

		prev->RemovePlayer(p.ID());
		World::GetRoom(p.Coords())->AddPlayer(p.ID());


		SendRoom(yellow + p.Name() + " warps away!",
			*prev);

		p.SendString(yellow + "You warp to a new location!");
		p.SendString(PrintRoom(p.CurrentRoom()));
		return;
	}
	
	if (firstword == "buyroom")
	{
		if (p.CurrentRoom()->GetBaseType() == RoomBaseType::COLLECTING)
		{
			CollectingRoom* cRoom = dynamic_cast<CollectingRoom*>(p.CurrentRoom().get());

			// Check if you already own it.
			if (cRoom->Owner() == p.Name())
			{
				p.SendString(red + "You already own this room!");
				return;
			}

			resource cost = cRoom->CurrentCost();

			if (p.GetResources()[cRoom->GetResourceType()] < cost)
			{
				p.SendString(red + "You don't have enough resources to buy this room!");
				return;
			}

			// Buy the room.
			p.GetResources()[cRoom->GetResourceType()] -= cost;
			cRoom->Owner() = p.Name();
			cRoom->BoughtTimes() += 1;

			p.SendString(green + "Successfully bought room for " + std::to_string(cost) + " " + ResourceTypeStrings[cRoom->GetResourceType()] + "!");
			return;
		}
		// Can't buy this room.
		p.SendString(red + "You can't buy a room of this type!");
		return;
	}

    // ------------------------------------------------------------------------
    //  GOD access commands
    // ------------------------------------------------------------------------
		if (firstword == "kick")
    {
			if (p.Rank() >= GOD)
			{
        // find a player to kick
        PlayerDatabase::iterator itr = 
		PlayerDatabase::findloggedin(ParseWord(p_data, 1));

		if (itr == PlayerDatabase::end())
        {
			p.SendString(red + bold + "Player could not be found.");
            return;
        }

		if (itr->Rank() > p.Rank())
        {
			p.SendString(red + bold + "You can't kick that player!");
            return;
        }
            
        itr->Conn()->Close();
		LogoutMessage(itr->Name() + " has been kicked by " +
			titledName + "!!!");
				return;
			}
			p.SendString(red + bold + "You don't have the right permissions to do that!");
			return;
		}
		//mute a player for a certain length of time
		if (firstword == "mute")
		{
			if (p.Rank() >= GOD)
			{
				PlayerDatabase::iterator itr =
					PlayerDatabase::findloggedin(ParseWord(p_data, 1));

				if (itr == PlayerDatabase::end())
				{
					p.SendString(red + bold + "Player could not be found.");
        return;
    }

				if (itr->Rank() > p.Rank())
				{
					p.SendString(red + bold + "You can't mute that player!");
					return;
				}

				itr->Mute() = true;
				Announce(itr->Name() + " has been muted by " +
					titledName + "!!!");

				return;
			}
			p.SendString(red + bold + "You don't have the right permissions to do that!");
			return;
		}

	//mute a player for a certain length of time
		if (firstword == "unmute")
		{
			if (p.Rank() >= GOD)
			{
				PlayerDatabase::iterator itr =
					PlayerDatabase::findloggedin(ParseWord(p_data, 1));

				if (itr == PlayerDatabase::end())
				{
					p.SendString(red + bold + "Player could not be found.");
					return;
				}

				if (itr->Rank() > p.Rank())
	{
					p.SendString(red + bold + "You can't unmute that player!");
					return;
	}

				itr->Mute() = false;
				Announce(itr->Name() + " has been unmuted by " +
					titledName + "!!!");
				return;
			}
			p.SendString(red + bold + "You don't have the right permissions to do that!");
			return;
		}
    // ------------------------------------------------------------------------
    //  ADMIN access commands
    // ------------------------------------------------------------------------
	//Make a System wide Broadcast
		if (firstword == "announce")
	{
			if (p.Rank() >= ADMIN)
		Announce(RemoveWord(p_data, 0));
		return;
	}

	// Ban a player
		if (firstword == "ban")
		{
			if (p.Rank() >= ADMIN)
	{
		string secondword = ParseWord(p_data, 1);
		if (secondword == "")
		{
			p.SendString(red + "Did not detect a player's name!");
			return;
		}
		PlayerDatabase::iterator target =
			PlayerDatabase::findloggedin(secondword);

		if (target == PlayerDatabase::end())
		{
			p.SendString(red + bold + "Could not find a player of that name online!");
			return;
		}

		// Ban the player.
		ConnectionManager<Telnet, Game>::BanIP(GetIPString(target->Conn()->GetRemoteAddress()));

		LogoutMessage(target->Name() + " has been banned by " +
			titledName + "!!!");

		target->Conn()->Close();
				return;
			}
			p.SendString(red + bold + "You don't have the right permissions to do that!");
		return;
	}

	//promote up one level
	//needs to be changed to change syntax to
	//Syntax (promote <player_name>)

		if (firstword == "promote")
		{
			if (p.Rank() >= ADMIN)
    {
		string name = ParseWord(p_data, 1);

		PlayerDatabase::iterator itr = PlayerDatabase::find(name);

		if (itr == PlayerDatabase::end())
        {
			p.SendString(red + bold + "Error: Could not find user " +
				name);
            return;
        }

		PlayerRank rank = GetRank(ParseWord(p_data, 2));

        itr->Rank() = rank;
		SendGame(green + bold + itr->Name() +
                  "'s rank has been changed to: " +
			GetRankString(rank));
		return;
	}
			p.SendString(red + bold + "You don't have the right permissions to do that!");
			return;
		}
	//reverse of above method
		if (firstword == "demote")
		{
			if (p.Rank() >= ADMIN)
	{
		string name = ParseWord(p_data, 1);

		PlayerDatabase::iterator itr = PlayerDatabase::find(name);

		if (itr == PlayerDatabase::end())
		{
			p.SendString(red + bold + "Error: Could not find user " +
				name);
        return;
    }

		PlayerRank rank = GetRank(ParseWord(p_data, 2));

		itr->Rank() = rank;
		SendGame(green + bold + itr->Name() +
			"'s rank has been changed to: " +
			GetRankString(rank));
		return;
	}
			p.SendString(red + bold + "You don't have the right permissions to do that!");
			return;
		}

	//reload game to add rooms w/out reseting server
		if (firstword == "reload")
		{
			if (p.Rank() >= ADMIN)
    {
		string db = BasicLib::LowerCase(ParseWord(p_data, 1));

		if (db == "player")
        {
			string user = ParseWord(p_data, 2);
			PlayerDatabase::iterator itr = PlayerDatabase::findfull(user);

			if (itr == PlayerDatabase::end())
            {
				p.SendString(bold + red + "Invalid Player Name");
            }
            else
            {
                bool a = itr->Active();
				if (a)     itr->Conn()->Handler()->Leave();
				PlayerDatabase::LoadPlayer(itr->Name());
				if (a)     itr->Conn()->Handler()->Enter();

				p.SendString(bold + cyan + "Player " +
					itr->Name() + " Reloaded!");
            }
        }
		else if (db == "rooms")
        {
					p.SendString(bold + cyan + "Room Template Database Reloaded!");
        }
        else
        {
			p.SendString(bold + red + "Invalid Database Name");
        }
				return;
			}
			p.SendString(red + bold + "You don't have the right permissions to do that!");
        return;
    }

		if (firstword == "shutdown")
		{
			if (p.Rank() >= ADMIN)
    {
		Announce("SYSTEM IS SHUTTING DOWN");
        Game::Running() = false;
        return;
    }
			p.SendString(red + bold + "You don't have the right permissions to do that!");
			return;
		}


	string prev = p_data;
	// Attempt to translate the command into something meaningful.
	// Only proceed if there is a difference
	string translated = m_dictionary.Translate(p_data);
	if (translated != prev)
	{
		Handle(translated);
		return;
	}

	// Do the same for the users dictionary.
	translated = p.GetDict().Translate(p_data);
	if (translated != prev)
	{
		Handle(translated);
		return;
	}

	for (auto player : PlayerDatabase::GetAllPlayers())
	{
		translated = player.GetDict().Translate(p_data);
		if (translated != prev)
		{
			p.SendString(red + "This command is not recognised, but another player, '" + player.Name() + "', has set up a shortcut for this command.\r\n" +
				"You can create your own shortcuts by entering 'rebind'...");
				return;
		}
	}


	// Possibly check all the other players dictionaries.
	// for p in players..
		// if translated != prev -> Someone has a map for this.

    // ------------------------------------------------------------------------
    //  Command not recognized, send to room
    // ------------------------------------------------------------------------
	// If command is not in dictionary Say unrecognised.
	// If command is a commonly used shortcut inform player of rebind ability.
	//SendRoom( bold + titledName + " says: " + dim + p_data, p.CurrentRoom() );

	p.SendString(bold + red + "Invalid Command!");
}


// ------------------------------------------------------------------------
//  This notifies the handler that there is a new connection
// ------------------------------------------------------------------------
void Game::Enter()
{
    USERLOG.Log(  
			GetIPString(m_connection->GetRemoteAddress()) +
        " - User " + "[" + GetTitleString((m_player)->GetPlayerTitle()) + "] " + (m_player)->Name() +
			" entering Game state.");

	InitDict();

    m_lastcommand = "";

    Player& p = *m_player;
	// Rooms broken right now. Will probably not be a pointer either.
    //p.CurrentRoom().AddPlayer( p.ID() );
    p.Active() = true;
    p.LoggedIn() = true;

		SendGame(bold + green + titledName + " has re-entered the game.");

	p.CurrentRoom()->AddPlayer(p.ID());

		p.SendString(PrintRoom(p.CurrentRoom()));
}

void Game::Leave()
{
    USERLOG.Log(  
			GetIPString(m_connection->GetRemoteAddress()) +
        " - User " + "[" + GetTitleString((m_player)->GetPlayerTitle()) + "] " + (m_player)->Name() +
			" leaving Game state.");

    // remove the player from his room
	// Rooms broken right now. Will probably not be a pointer either.
		m_player->CurrentRoom()->RemovePlayer(m_player);
    m_player->Active() = false;

    // log out the player from the database if the connection has been closed
		if (m_connection->Closed())
    {
			PlayerDatabase::Logout(m_player);
    }
}


// ------------------------------------------------------------------------
//  This notifies the handler that a connection has unexpectedly hung up.
// ------------------------------------------------------------------------
void Game::Hungup()
{
    USERLOG.Log(  
			GetIPString(m_connection->GetRemoteAddress()) +
        " - User " + "[" + GetTitleString((m_player)->GetPlayerTitle()) + "] " + (m_player)->Name() +
			" hung up.");

    Player& p = *m_player;
		LogoutMessage(p.Name() + " has suddenly disappeared from the game.");
}

// ------------------------------------------------------------------------
//  This notifies the handler that a connection is being kicked due to 
//  flooding the server.
// ------------------------------------------------------------------------
void Game::Flooded()
{
    USERLOG.Log(  
			GetIPString(m_connection->GetRemoteAddress()) +
        " - User " + "[" + GetTitleString((m_player)->GetPlayerTitle()) + "] " + (m_player)->Name() +
			" flooded.");

    Player& p = *m_player;
		LogoutMessage(titledName + " has been kicked for flooding!");
}

// ------------------------------------------------------------------------
//  Sends a string to everyone connected.
// ------------------------------------------------------------------------
	void Game::SendGlobal(const string& p_str)
{
		operate_on_if(PlayerDatabase::begin(),
                   PlayerDatabase::end(),
			playersend(p_str),
			playerloggedin());
}

// ------------------------------------------------------------------------
//  Sends a string to everyone "within the game"
// ------------------------------------------------------------------------
	void Game::SendGame(const std::string& p_str)
{
		operate_on_if(PlayerDatabase::begin(),
                   PlayerDatabase::end(),
			playersend(p_str),
			playeractive());
}

// ------------------------------------------------------------------------
//  Sends a logout message
// ------------------------------------------------------------------------
	void Game::LogoutMessage(const string& p_reason)
{
		SendGame(SocketLib::red + SocketLib::bold + p_reason);
}

// ------------------------------------------------------------------------
//  Sends a system announcement
// ------------------------------------------------------------------------
void Game::Announce(const string& p_announcement)
{
	SendGlobal(SocketLib::cyan + SocketLib::bold +
		"System Announcement: " + p_announcement);
}

// ------------------------------------------------------------------------
//  Sends a system announcement
// ------------------------------------------------------------------------
void Game::Sponsor(const string& p_announcement)
{
		SendGlobal(SocketLib::blue + SocketLib::bold +
		"Sponsored By: " + p_announcement);
}

// ------------------------------------------------------------------------
//  Sends a whisper string to the requested player
// ------------------------------------------------------------------------
	void Game::Whisper(std::string p_str, std::string p_player)
{
    // find the player
		PlayerDatabase::iterator itr = PlayerDatabase::findactive(p_player);
    
    // if no match was found
		if (itr == PlayerDatabase::end())
    {
			m_player->SendString(red + bold + "Error, cannot find user.");
    }
    else
    {
			itr->SendString(magenta + m_player->Name() + " whispers to you: " +
				reset + p_str);

			m_player->SendString(magenta + "You whisper to " + itr->Name() +
				": " + reset + p_str);
    }
}

// ------------------------------------------------------------------------
//  Functor that generates a who-listing for a single player
// ------------------------------------------------------------------------
struct wholist
{
    string str;

		void operator() (Player& p)
    {
			str += " " + tostring(p.Name(), 17) + "| ";
        
			if (p.Active())
            str += green + "Online  " + white;
			else if (p.LoggedIn())
            str += yellow + "Inactive" + white;
        else
            str += red + "Offline " + white;

        str += " | ";
			switch (p.Rank())
        {
            case REGULAR:   str += white;   break;
            case GOD:       str += yellow;  break;
            case ADMIN:     str += green;   break;
        }

			str += GetRankString(p.Rank());

        str += white + "\r\n";
    }
};

// ------------------------------------------------------------------------
//  This prints up the who-list for the game.
// ------------------------------------------------------------------------
	string Game::WhoList(const string& p_who)
{
    using namespace BasicLib;

    string str = white + bold +
        "--------------------------------------------------------------------------------\r\n" + 
        " Name             | Activity | Rank\r\n" + 
        "--------------------------------------------------------------------------------\r\n";

    wholist who;
		if (p_who == "all")
    {
        who = BasicLib::operate_on_if( 
                        PlayerDatabase::begin(),
                        PlayerDatabase::end(),
                        wholist(),
				always<Player>());
    }
    else
    {
        who = BasicLib::operate_on_if( 
                        PlayerDatabase::begin(),
                        PlayerDatabase::end(),
                        wholist(),
				playerloggedin());
    }

    str += who.str;

    
    str +=
        "--------------------------------------------------------------------------------";

    return str;
}

// ------------------------------------------------------------------------
//  Prints out a help listing based on a user's rank.
// ------------------------------------------------------------------------
	string Game::PrintHelp(PlayerRank p_rank)
{
	// Commented out stuff that isn't implemented yet to reduce confusion.
	static string help = white + bold +
		"--------------------------------- Command List ------------------------------------\r\n" +
		" /                          - Repeat last command\r\n" +
		" <direction>                - Move in a direction (north, n, south, e, west etc.)\r\n" +
		" buyroom                    - Buy current collecting room\r\n" +
		" change <title>             - Change current title to <title>\r\n" +
		" collect, c                 - Collect resources from room.\r\n" +
		" corp <mesg>                - Send message to corporation\r\n" +
		" exit, quit                 - Leave the game.\r\n" +
		" global <mesg>              - Send message to everyone\r\n" +
		" help                       - Shows this menu\r\n" +
		" interact, i                - Interact with a Special Room\r\n" +
		" look, l                    - Show description of room\r\n" +
		" invite                     - Invite a player to a corporation you are a leader of\r\n" +
		" leavecorp                  - Leave your current corporations\r\n" +
		" leaderboards		         - Display a certain leaderboard\r\n" +
		" rebind                     - Create new shortcuts for a command\r\n" +
		" say <mesg>                 - Send message to current room\r\n" +
		" shortcuts                  - Display all available shortcuts\r\n" +
		" shout <mesg>               - Send message to adjacent rooms\r\n" +
		" stats                      - Shows stats\r\n" +
		" time                       - Show current system time\r\n" +
		" titles                     - Show all available titles\r\n" +
		" trade <player><amount><res>- Send an amount of a resource to another player\r\n" +
		" warp <x> <y>               - Warp to the coordinates of a discovered room\r\n" +
		" whisper <who> <msg>        - Send private message\r\n" +
		" who                        - Show list of everyone online\r\n" +
		" who all                    - Show list of everyone\r\n";


	static string god = cyan + bold +
		"--------------------------------- God Commands ------------------------------------\r\n" +
		" kick <who>                 - Kick user from the game\r\n" +
		" mute <who> <duration>		 - Mute chat from player for <duration>\r\n";
	
    static string admin = blue + bold +
        "-------------------------------- Admin Commands -----------------------------------\r\n" + 
        " announce <msg>             - Makes a global system announcement\r\n" +
		" ban <who>                  - Ban a players Ip from the server\r\n" +
		" demote <who>			     - Changes a player to REGULAR rank\r\n" +
		" promote <who> <rank>       - Changes a player to GOD rank\r\n" +
		" reload <db>                - Reloads the requested database\r\n" +
        " shutdown                   - Shuts the server down\r\n";

    static string end = white + bold + 
        "-----------------------------------------------------------------------------------";


		if (p_rank == REGULAR)
        return help + end;
		else if (p_rank == GOD)
        return help + god + end;
		else if (p_rank == ADMIN)
        return help + god + admin + end;
    else return "ERROR";

}

//	-----------------------------------------------------------------------
//	Display the players rebound shortcuts
//	-----------------------------------------------------------------------
string Game::PrintShortcuts()
{
	string shortcuts = white + bold +
		"-------------------------------- Your Shortcuts --------------------------------\r\n" +
		"-Command-                    -Shortcut-\r\n";
	std::map<string, string> temp = m_player->GetDict().GetDictionary();
	std::map<string, string>::iterator mitr = temp.begin();
	for (mitr; mitr != temp.end(); ++mitr)
	{
		shortcuts += white + BufferWord(mitr->second, 29) + mitr->first + "\r\n";
	}
	temp = m_dictionary.GetDictionary();
	mitr = temp.begin();
	for (mitr; mitr != temp.end(); ++mitr)
	{
		shortcuts += white + BufferWord(mitr->second, 29) + mitr->first + "\r\n";
	}

	shortcuts += 
		"--------------------------------------------------------------------------------\r\n";
	return shortcuts;
}

// ------------------------------------------------------------------------
//  This prints up the stats of the player
// ------------------------------------------------------------------------
string Game::PrintStats()
{
    using namespace BasicLib;
    Player& p = *m_player;

    return white + bold +
        "---------------------------------- Your Stats ----------------------------------\r\n" + 
        " Name:\t" + p.Name() + "\r\n" +
			" Rank:\t" + GetRankString(p.Rank()) + "\r\n" +
		" Corp:\t" + p.CorpName() + "\tLeader:\t" + ((p.CorpLeader()) ? "Yes" : "No") + "\r\n" +
		" Soul:\t" + ((p.HasSoul()) ? "Yes" : "No") + "\r\n" +
		"\tAmount\tItemLvl" + "\r\n" +
		" Wood:\t" + std::to_string(p.GetResources()[WOOD]) + "\t" + std::to_string(p.GetItemLevels()[WOOD]) + "\r\n" +
		" Stone:\t" + std::to_string(p.GetResources()[STONE]) + "\t" + std::to_string(p.GetItemLevels()[STONE]) + "\r\n" +
		" Iron:\t" + std::to_string(p.GetResources()[IRON]) + "\t" + std::to_string(p.GetItemLevels()[IRON]) + "\r\n" +
		" Gold:\t" + std::to_string(p.GetResources()[GOLD]) + "\t" + std::to_string(p.GetItemLevels()[GOLD]) + "\r\n" +
        "--------------------------------------------------------------------------------";
}

	string Game::PrintRoom(std::shared_ptr<Room> p_room)
{

    string desc = "\r\n" + bold + yellow + p_room->Name() + "\r\n";
    string temp;
    int count;

    desc += bold + yellow + p_room->Description() + "\r\n";

	// If the room is a collecting room, also want to include the owner and current cost of the room.
	if (p_room->GetBaseType() == RoomBaseType::COLLECTING)
	{
		// Need access to the collecting room specific data.
		CollectingRoom* cRoom = dynamic_cast<CollectingRoom*>(p_room.get());

		desc += yellow + "Owner: " + cRoom->Owner() + 
			" -- Current Cost: " + std::to_string(cRoom->CurrentCost()) +
			" " + ResourceTypeStrings[cRoom->GetResourceType()] + newline;
		
	}

    // ---------------------------------
    // PEOPLE
    // ---------------------------------
    temp = bold + cyan + "People: " + white;
    count = 0;
    std::list<player>::iterator playeritr = p_room->Players().begin();
		while (playeritr != p_room->Players().end())
    {
		temp += "[" + GetTitleString((*playeritr)->GetPlayerTitle()) + "] " + (*playeritr)->Name() + ", ";
        count++;
        ++playeritr;
    }

		if (count > 0)
    {
			temp.erase(temp.size() - 2, 2);
        desc += temp + "\r\n";
    }
    return desc;
}

	void Game::SendRoom(string p_text, Room p_room)
{
		std::for_each(p_room.Players().begin(),
                   p_room.Players().end(),
			playersend(p_text));
}



	void Game::Move(int p_direction)
{
    Player& p = *m_player;
	std::shared_ptr<Room> prev = p.CurrentRoom();

	// Right
	vector2 dir;
	

	if (p_direction == EAST)
		dir = vector2(1, 0);
	else if (p_direction == NORTH)
		dir = vector2(0, 1);
	else if (p_direction == WEST)
		dir = vector2(-1, 0);
	else if (p_direction == SOUTH)
		dir = vector2(0, -1);
	else
		dir = vector2();

	vector2 temp = World::ChangeRoom(p.Coords(), dir);
	if (temp.x <= WorldSize && temp.y <= WorldSize)
	{
		p.Coords() = temp;

		prev->RemovePlayer(p.ID());
		World::GetRoom(p.Coords())->AddPlayer(p.ID());

		SendRoom(yellow + p.Name() + " leaves to the " +
			DIRECTIONSTRINGS[p_direction] + ".",
			*prev);

		p.SendString(yellow + "You walk " + DIRECTIONSTRINGS[p_direction] + ".");
		p.SendString(PrintRoom(p.CurrentRoom()));

		if (m_player->Coords().length() <= 100)
			m_player->AddTitle(TRAVELLER);
	}
	else
	{
		p.SendString(yellow + "You have reached the end of the world. Careful Now!");
	}
}

void SimpleMUD::Game::Collect()
{
	Player& p = *m_player;

	if (p.CurrentRoom()->GetBaseType() == RoomBaseType::COLLECTING)
	{
		
		// Get the value from the Room
		CollectingRoom* cRoom = dynamic_cast<CollectingRoom*>(p.CurrentRoom().get());

		// Get the type from the room.
		ResourceType type = cRoom->GetResourceType();

		// If you have an item of this type of at least level one, you can collect.
		if (p.GetItemLevels()[type] > 0)
		{
			resource reward = cRoom->Collect();

			// Bonus based on your item level, which must be at least 1.
			int itemLevel = p.GetItemLevels()[type];
			reward *= itemLevel;

			// Add any bonuses due is the player is the owner of the room.
			if (cRoom->Owner() != OWNERNONE)
			{
				if (cRoom->Owner() == p.Name())
				{
					// Just doubling the resources gotten from the room.
					// Could do alternate benefits here.
					reward = resource(reward * 2);
				}
				// If someone from the same corporation as the player owns the land.
				// Get a smaller benefit.
				else
				{
					auto owner = PlayerDatabase::findfull(cRoom->Owner());
					auto guildMembers = PlayerDatabase::CorpMembers(owner->CorpName());
					for (auto mem : guildMembers)
					{
						if (mem->Name() == p.Name())
						{
							// In the same guild. Add bonus.
							reward = resource(reward * 1.5f);
							break;
						}
					}
					// Not in the same guild as the owner, no additional benefits.
					// Could add reductions here.
				}
			}

			// Add that to the players totals.
			p.GetResources()[type] += reward;
			
			if (p.GetResources()[type] >= 1000)
			{
				if (type == WOOD)
				{
					p.SendString(green + "Achievement Unlocked! " + GetTitleString(LUMBERJACK));
					p.AddTitle(LUMBERJACK);
				}
				if (type == STONE)
				{
					p.SendString(green + "Achievement Unlocked! " + GetTitleString(STONEMASON));
					p.AddTitle(STONEMASON);
				}
				if (type == IRON)
				{
					p.SendString(green + "Achievement Unlocked! " + GetTitleString(BLACKSMITH));
					p.AddTitle(BLACKSMITH);
				}
				if (type == GOLD)
				{
					p.SendString(green + "Achievement Unlocked! " + GetTitleString(GOLDMINER));
					p.AddTitle(GOLDMINER);
				}
			}

			p.SendString(green + "Collected " + std::to_string(reward) + " " + ResourceTypeStrings[type] + "!");
			if (p.FirstOfType()[type])
			{
				p.FirstOfType()[type] = false;
				if (type == WOOD)
				{
					p.SendString(green + "Achievement Unlocked! " + GetTitleString(GOTWOOD));
					p.AddTitle(GOTWOOD);
				}
				else if (type == STONE)
				{
					p.SendString(green + "Achievement Unlocked! " + GetTitleString(STONECOLD));
					p.AddTitle(STONECOLD);
				}
				else if (type == IRON)
				{
					p.SendString(green + "Achievement Unlocked! " + GetTitleString(IRONMAIDEN));
					p.AddTitle(IRONMAIDEN);
				}
				else if (type == GOLD)
				{
					p.SendString(green + "Achievement Unlocked! " + GetTitleString(GOLDDIGGER));
					p.AddTitle(GOLDDIGGER);
				}
			}
		}
		else
		{
			p.SendString(red + "You need an item of at least level 1 for this resource type to collect it!");
		}		
	}
	else
	{
		// Cant collect in this type of room.
		p.SendString(red + "You can't collect in this room!");
	}
}

void Game::InitDict()
{
	if (m_dictInitialized == false)
	{
		// Add any stuff to the dictionary for the game.
		m_dictionary.AddCommandPair("north", "n");
		m_dictionary.AddCommandPair("south", "s");
		m_dictionary.AddCommandPair("east", "e");
		m_dictionary.AddCommandPair("west", "w");
		m_dictionary.AddCommandPair("collect", "c");
		m_dictionary.AddCommandPair("interact", "i");
		m_dictionary.AddCommandPair("look", "l");

		m_dictInitialized = true;
	}
}

//
//
//string Game::StoreList( entityid p_store )
//{
//    Store& s = StoreDatabase::get( p_store );
//
//    string output = white + bold + 
//              "--------------------------------------------------------------------------------\r\n";
//    output += " Welcome to " + s.Name() + "!\r\n";
//    output += "--------------------------------------------------------------------------------\r\n";
//    output += " Item                           | Price\r\n";
//    output += "--------------------------------------------------------------------------------\r\n";
//
//    Store::iterator itr = s.begin();
//    while( itr != s.end() )
//    {
//        output += " " + tostring( (*itr)->Name(), 31 ) + "| ";
//        output += tostring( (*itr)->Price() ) + "\r\n";
//        ++itr;
//    }
//    output += bold + 
//              "--------------------------------------------------------------------------------\r\n";
//
//    return output;
//}


}   // end namespace SimpleMUD
