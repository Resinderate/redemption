// MUD Programming
// Ron Penton
// (C)2003
// Game.cpp - This class is the game handler for SimpleMUD.
// 
// 

#include "Logon.h"
#include "Game.h"
#include "PlayerDatabase.h"
#include "../BasicLib/BasicLib.h"

using namespace SocketLib;
using namespace BasicLib;
using std::string;

#define titledName "[" + GetTitleString(p.GetPlayerTitle()) + "] " + p.Name()
namespace SimpleMUD
{


// declare the static instance of the timer
BasicLib::Timer Game::s_timer;
bool Game::s_running = false;

// ------------------------------------------------------------------------
//  This handles incomming commands. Anything passed into this function
//  is assumed to be a complete command from a client.
// ------------------------------------------------------------------------
void Game::Handle( string p_data )
{
    Player& p = *m_player;

    // check if the player wants to repeat a command
    if( p_data == "/" )
    {
        p_data = m_lastcommand;
    }
    else
    {
        // if not, record the command.
        m_lastcommand = p_data;
    }

    // get the first word and lowercase it.
    string firstword = BasicLib::LowerCase( ParseWord( p_data, 0 ) );



    // ------------------------------------------------------------------------
    //  REGULAR access commands
	//	@author: Kevin Duffy
    // ------------------------------------------------------------------------
	// message sent to current Tile
	if (firstword == "say")
    {
		string text = RemoveWord(p_data, 0);
		SendGame(magenta + bold + titledName + " -> Local: " + white + text);
        return;
    }

	// message sent to current and adjacent Tiles
	if (firstword == "shout")
    {
		string text = RemoveWord(p_data, 0);
		SendGame(magenta + bold + titledName + " -> Neighbour:  " + white + text);
        return;
    }

	// message sent to all corporation members
	if (firstword == "corp")
    {
		string text = RemoveWord(p_data, 0);
		SendGame(magenta + bold + titledName + " -> Corporation: " + white + text);
        return;
    }

	// message sent to whole server
	if (firstword == "global")
	{
		string text = RemoveWord(p_data, 0);
		SendGame(magenta + bold + titledName + " -> Global: " + white + text);
		return;
	}

	//Private message
	if (firstword == "whisper")
    {
		// get the players name
		string name = ParseWord(p_data, 1);
		string message = RemoveWord(RemoveWord(p_data, 0), 0);

		Whisper(message, name);
		return;
	}

	//display all available commands
	if (firstword == "help")
	{
		p.SendString(PrintHelp(p.Rank()));
		return;
	}

	//leave the game
	if (firstword == "exit")
	{
        m_connection->Close();
		LogoutMessage(titledName + " has left the realm.");
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
			+ s_timer.GetString() + ".");
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

	//movement 
	if (firstword == "go")
    {
		string secondword = ParseWord(p_data, 1);
		if (secondword == "north")
		{
			Move(NORTH);
        return;
    }
		if (secondword == "east")
    {
			Move(EAST);
        return;
    }
		if (secondword == "south")
    {
			Move(SOUTH);
        return;
    }
		if (secondword == "west")
    {
			Move(WEST);
        return;
    }
		else
    {
        return;
    }
	}

	//Display players titles
	if (firstword == "titles")
    {
		string temp = "Available Titles: ";
		std::list<PlayerTitle>::iterator itr = p.Titles().begin();
		for (itr; itr != p.Titles().end(); ++itr)
		{
			PlayerTitle t = *itr;
			temp += GetTitleString(t) + ", ";
		}

			temp.erase(temp.size() - 2, 2);
			temp += "\r\n";

			p.SendString(green + bold + temp);
            return;
        }

	//Change players title
	if (firstword == "change")
	{
		//Syntax (change <title_name>)

		string secondword = ParseWord(p_data, 1);
		p.SetTitle(secondword);

		p.SendString(green + bold + "Title changed to [" + secondword + "]");
		return;
	}

	//Rebind a command to a shortcut key
	if (firstword == "rebind")
	{
		//Syntax (rebind <command> <shortcut>)
	}

	//If in a resource room collect whatever resource is there
	if (firstword == "collect" /*&& current room == resource room*/)
	{

	}

	//give a player a certain amount of a resource
	if (firstword == "trade")
	{
		//Syntax (trade <player_name> <amount> <resource_type>)
	}

	//complex Special room command Prompts a dialogue for the player to engage with (Devil Room, Corp Room, Assasin Room)
	if (firstword == "interact")
	{

	}

	//Invite a player to your corporation as long as you are the leader/ have permissions
	if (firstword == "invite" /*&& check permissions / is part of corp */)
    {
		//Syntax (invite <player_name>)
	}

	//leave current corporation
	if (firstword == "leave")
	{
		/*check is part of corp */
        }

	//check position certain leaderboard (Player's Corporation/World Rank, Resources Gathered, Corporation Souls Redeemed, Corporation's Resource Rank)
	if (firstword == "leaderboard")
	{
		//Syntax (invite <leaderboard_type>)
	}

    // ------------------------------------------------------------------------
    //  GOD access commands
    // ------------------------------------------------------------------------
	if (firstword == "kick" && p.Rank() >= GOD)
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

	//mute a player for a certain length of time
	if (firstword == "mute" && p.Rank() >= GOD)
	{
		//Syntax (mute <player_name> <mute_duration>)
	}

    // ------------------------------------------------------------------------
    //  ADMIN access commands
    // ------------------------------------------------------------------------
	//Make a System wide Broadcast
	if (firstword == "announce" && p.Rank() >= ADMIN)
    {
		Announce(RemoveWord(p_data, 0));
        return;
    }

	//promote up one level
	//needs to be changed to change syntax to
	//Syntax (promote <player_name>)

	if (firstword == "promote" && p.Rank() >= ADMIN)
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
	//reverse of above method
	if (firstword == "demote" && p.Rank() >= ADMIN)
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

	//reload game to add rooms w/out reseting server
	if (firstword == "reload" && p.Rank() >= ADMIN)
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
            p.SendString( bold + cyan + "Room Template Database Reloaded!" );
        }
        else
        {
			p.SendString(bold + red + "Invalid Database Name");
        }
        return;
    }

	if (firstword == "shutdown" && p.Rank() >= ADMIN)
    {
		Announce("SYSTEM IS SHUTTING DOWN");
        Game::Running() = false;
        return;
    }


    // ------------------------------------------------------------------------
    //  Command not recognized, send to room
    // ------------------------------------------------------------------------
	// If command is not in dictionary Say unrecognised.
	// If command is a commonly used shortcut inform player of rebind ability.
	SendRoom( bold + titledName + " says: " + dim + p_data, p.CurrentRoom() );

}


// ------------------------------------------------------------------------
//  This notifies the handler that there is a new connection
// ------------------------------------------------------------------------
void Game::Enter()
{
    USERLOG.Log(  
        GetIPString( m_connection->GetRemoteAddress() ) + 
        " - User " + "[" + GetTitleString((m_player)->GetPlayerTitle()) + "] " + (m_player)->Name() +
        " entering Game state." );

    m_lastcommand = "";

    Player& p = *m_player;
	// Rooms broken right now. Will probably not be a pointer either.
    //p.CurrentRoom().AddPlayer( p.ID() );
    p.Active() = true;
    p.LoggedIn() = true;

    SendGame( bold + green + titledName + " has entered the realm." );


    p.SendString( PrintRoom( p.CurrentRoom() ) );
}

void Game::Leave()
{
    USERLOG.Log(  
        GetIPString( m_connection->GetRemoteAddress() ) + 
        " - User " + "[" + GetTitleString((m_player)->GetPlayerTitle()) + "] " + (m_player)->Name() +
        " leaving Game state." );

    // remove the player from his room
	// Rooms broken right now. Will probably not be a pointer either.
	//m_player->CurrentRoom()->RemovePlayer( m_player );
    m_player->Active() = false;

    // log out the player from the database if the connection has been closed
    if( m_connection->Closed() )
    {
        PlayerDatabase::Logout( m_player );
    }
}


// ------------------------------------------------------------------------
//  This notifies the handler that a connection has unexpectedly hung up.
// ------------------------------------------------------------------------
void Game::Hungup()
{
    USERLOG.Log(  
        GetIPString( m_connection->GetRemoteAddress() ) + 
        " - User " + "[" + GetTitleString((m_player)->GetPlayerTitle()) + "] " + (m_player)->Name() +
        " hung up." );

    Player& p = *m_player;
    LogoutMessage( p.Name() + " has suddenly disappeared from the realm." );
}

// ------------------------------------------------------------------------
//  This notifies the handler that a connection is being kicked due to 
//  flooding the server.
// ------------------------------------------------------------------------
void Game::Flooded()
{
    USERLOG.Log(  
        GetIPString( m_connection->GetRemoteAddress() ) + 
        " - User " + "[" + GetTitleString((m_player)->GetPlayerTitle()) + "] " + (m_player)->Name() +
        " flooded." );

    Player& p = *m_player;
    LogoutMessage( titledName + " has been kicked for flooding!" );
}

// ------------------------------------------------------------------------
//  Sends a string to everyone connected.
// ------------------------------------------------------------------------
void Game::SendGlobal( const string& p_str )
{
    operate_on_if( PlayerDatabase::begin(),
                   PlayerDatabase::end(),
                   playersend( p_str ),
                   playerloggedin() );
}

// ------------------------------------------------------------------------
//  Sends a string to everyone "within the game"
// ------------------------------------------------------------------------
void Game::SendGame( const std::string& p_str )
{
    operate_on_if( PlayerDatabase::begin(),
                   PlayerDatabase::end(),
                   playersend( p_str ),
                   playeractive() );
}

// ------------------------------------------------------------------------
//  Sends a logout message
// ------------------------------------------------------------------------
void Game::LogoutMessage( const string& p_reason )
{
    SendGame( SocketLib::red + SocketLib::bold + p_reason );
}

// ------------------------------------------------------------------------
//  Sends a system announcement
// ------------------------------------------------------------------------
void Game::Announce( const string& p_announcement )
{
    SendGlobal( SocketLib::cyan + SocketLib::bold + 
                "System Announcement: " + p_announcement );
}

// ------------------------------------------------------------------------
//  Sends a whisper string to the requested player
// ------------------------------------------------------------------------
void Game::Whisper( std::string p_str, std::string p_player )
{
    // find the player
    PlayerDatabase::iterator itr = PlayerDatabase::findactive( p_player );
    
    // if no match was found
    if( itr == PlayerDatabase::end() )
    {
        m_player->SendString( red + bold + "Error, cannot find user." );
    }
    else
    {
        itr->SendString( yellow + m_player->Name() + " whispers to you: " + 
                         reset + p_str );

        m_player->SendString( yellow + "You whisper to " + itr->Name() + 
                              ": " + reset + p_str );
    }
}

// ------------------------------------------------------------------------
//  Functor that generates a who-listing for a single player
// ------------------------------------------------------------------------
struct wholist
{
    string str;

    void operator() ( Player& p )
    {
        str += " " + tostring( p.Name(), 17 ) + "| ";        
        
        if( p.Active() )
            str += green + "Online  " + white;
        else if( p.LoggedIn() )
            str += yellow + "Inactive" + white;
        else
            str += red + "Offline " + white;

        str += " | ";
        switch( p.Rank() )
        {
            case REGULAR:   str += white;   break;
            case GOD:       str += yellow;  break;
            case ADMIN:     str += green;   break;
        }

        str += GetRankString( p.Rank() );

        str += white + "\r\n";
    }
};

// ------------------------------------------------------------------------
//  This prints up the who-list for the realm.
// ------------------------------------------------------------------------
string Game::WhoList( const string& p_who )
{
    using namespace BasicLib;

    string str = white + bold +
        "--------------------------------------------------------------------------------\r\n" + 
        " Name             | Level     | Activity | Rank\r\n" + 
        "--------------------------------------------------------------------------------\r\n";

    wholist who;
    if( p_who == "all" )
    {
        who = BasicLib::operate_on_if( 
                        PlayerDatabase::begin(),
                        PlayerDatabase::end(),
                        wholist(),
                        always<Player>() );
    }
    else
    {
        who = BasicLib::operate_on_if( 
                        PlayerDatabase::begin(),
                        PlayerDatabase::end(),
                        wholist(),
                        playerloggedin() );
    }

    str += who.str;

    
    str +=
        "--------------------------------------------------------------------------------";

    return str;
}

// ------------------------------------------------------------------------
//  Prints out a help listing based on a user's rank.
// ------------------------------------------------------------------------
string Game::PrintHelp( PlayerRank p_rank )
{
	static string help = white + bold +
		"--------------------------------- Command List ---------------------------------\r\n" +
		" /                          - Repeats your last command exactly.\r\n" +
		" chat <mesg>                - Sends message to everyone in the game\r\n" +
		" experience                 - Shows your experience statistics\r\n" +
		" help                       - Shows this menu\r\n" +
		" quit                       - Allows you to leave the realm.\r\n" +
		" stats                      - Shows all of your statistics\r\n" +
		" time                       - shows the current system time.\r\n" +
		" whisper <who> <msg>        - Sends message to one person\r\n" +
		" who                        - Shows a list of everyone online\r\n" +
		" who all                    - Shows a list of everyone\r\n" +
		" look                       - Shows you the contents of a room\r\n" +
		" north/east/south/west      - Moves in a direction\r\n" +
		" train                      - Train to the next level (TR)\r\n" +
		" editstats                  - Edit your statistics (TR)\r\n";


    static string god = yellow + bold +
        "--------------------------------- God Commands ---------------------------------\r\n" + 
        " kick <who>                 - kicks a user from the realm\r\n";

    static string admin = green + bold +
        "-------------------------------- Admin Commands --------------------------------\r\n" + 
        " announce <msg>             - Makes a global system announcement\r\n" + 
        " changerank <who> <rank>    - Changes the rank of a player\r\n" + 
        " reload <db>                - Reloads the requested database\r\n" + 
        " shutdown                   - Shuts the server down\r\n";

    static string end = white + bold + 
        "--------------------------------------------------------------------------------";


    if( p_rank == REGULAR )
        return help + end;
    else if( p_rank == GOD )
        return help + god + end;
    else if( p_rank == ADMIN )
        return help + god + admin + end;
    else return "ERROR";

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
        " Name:          " + p.Name() + "\r\n" +
        " Rank:          " + GetRankString( p.Rank() ) + "\r\n" +
        "--------------------------------------------------------------------------------";
}

string Game::PrintRoom( Room p_room )
{

    string desc = "\r\n" + bold + white + p_room.Name() + "\r\n";
    string temp;
    int count;

    desc += bold + magenta + p_room.Description() + "\r\n";

    desc += "\r\n";
    // ---------------------------------
    // PEOPLE
    // ---------------------------------
    temp = bold + cyan + "People: ";
    count = 0;
    std::list<player>::iterator playeritr = p_room.Players().begin();
    while( playeritr != p_room.Players().end() )
    {
		temp += "[" + GetTitleString((*playeritr)->GetPlayerTitle()) + "] " + (*playeritr)->Name() + ", ";
        count++;
        ++playeritr;
    }

    if( count > 0 )
    {
        temp.erase( temp.size() - 2, 2 );
        desc += temp + "\r\n";
    }
    return desc;
}

void Game::SendRoom( string p_text, Room p_room )
{
    std::for_each( p_room.Players().begin(),
                   p_room.Players().end(),
                   playersend( p_text ) );
}



void Game::Move( int p_direction )
{
    Player& p = *m_player;
    Room previous = p.CurrentRoom();

	// Right
	p.Coords() = World::ChangeRoom(p.Coords(), vector2(1, 0));

    SendRoom( green + p.Name() + " leaves to the " + 
              DIRECTIONSTRINGS[p_direction] + ".",
              previous );

	p.SendString( yellow + "You walk " + DIRECTIONSTRINGS[p_direction] + "." );

	vector2 coords = p.Coords();
	string out = "New Coods: " + std::to_string(coords.x) + ", " + std::to_string(coords.y);
	p.SendString(yellow + out);
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
