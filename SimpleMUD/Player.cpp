// MUD Programming
// Ron Penton
// (C)2003
// Player.cpp - The class defining Players in the SimpleMUD
// 
// 

#include <cmath>
#include <string>
#include <fstream>
#include <vector>

#include "SimpleMUDLogs.h"
#include "Player.h"

namespace SimpleMUD
{

Player::Player()
{
    m_pass = "UNDEFINED";
    m_rank = REGULAR;

    m_connection = 0;
    m_loggedin = false;
    m_active = false;
    m_newbie = true;

	m_noOfTitles = 0;
	//m_availableTitles.push_back(PEASANT);
	AddTitle(PEASANT);
	SetTitle("Peasant");

	for (int i = 0; i < NumResourceType; i++)
	{
		m_resourceAmounts[i] = 0;
	}
	m_itemLevels[WOOD] = 1;
	m_itemLevels[STONE] = 0;
	m_itemLevels[IRON] = 0;
	m_itemLevels[GOLD] = 0;

	m_firstOfType[WOOD] = true;
	m_firstOfType[STONE] = true;
	m_firstOfType[IRON] = true;
	m_firstOfType[GOLD] = true;

	m_corp = CORPNONE;
	m_corpLeader = false;

	m_coords = vector2(0, 0);

	m_hasSoul = false;
	m_lastActivity = std::chrono::system_clock::now();
}

//	@author Kevin Duffy
//  Return a list of rooms adjacent to the player to 
std::list<vector2> Player::AdjacentRooms()
{
	std::list<vector2> ret;
	ret.push_back(vector2(m_coords.x + 0, m_coords.y + 0));
	ret.push_back(vector2(m_coords.x + 1, m_coords.y + 0));
	ret.push_back(vector2(m_coords.x + 0, m_coords.y + 1));
	ret.push_back(vector2(m_coords.x - 1, m_coords.y + 0));
	ret.push_back(vector2(m_coords.x + 0, m_coords.y - 1));
	return ret;
}

//	@author Kevin Duffy
//  Choose a players Title
bool Player::SetTitle(string p_str)
{
	//iterate through the list of player's titles and see if the title exists 
	//C++ 11
	for (auto title : m_availableTitles)
	{
		string temp = GetTitleString(title);
		if (temp == p_str)
		{
			m_title = title;
			return true;
		}
	}
	return false;
}
//	@author Kevin Duffy
//	Function to add a title to a players collection
//	For use later with 
void Player::AddTitle(PlayerTitle p_val)
{
	bool exists = false;
	//C++ 11
	for (auto title : m_availableTitles)
	{
		if (title == p_val)
		{
			exists = true;
		}
	}
	if (!exists)
	{
		m_availableTitles.push_back(p_val);
		m_noOfTitles++;
	}
}

// ------------------------------------------------------------------------
//  This sends a string to the players connection.
// ------------------------------------------------------------------------
void Player::SendString( const std::string& p_string )
{
    using namespace SocketLib;

    if( Conn() == 0 )
    {
        ERRORLOG.Log( "Trying to send string to player " +
                      Name() + " but player is not connected." );
        return;
    }

    // send the string, newline included.
    Conn()->Protocol().SendString( *Conn(), p_string + newline );

    if( Active() )
    {
        PrintStatbar();
    }
}

// ------------------------------------------------------------------------
//  This prints up the players "statbar", ie: his hitpoints.
// ------------------------------------------------------------------------
void Player::PrintStatbar( bool p_update )
{
    using namespace SocketLib;
    using namespace BasicLib;

    // if this is a statusbar update and the user is currently typing something,
    // then do nothing.
    if( p_update && Conn()->Protocol().Buffered() > 0 )
        return;

    string statbar = white + bold + ">";

    Conn()->Protocol().SendString( *Conn(), clearline + "\r" + statbar + reset );
}



// --------------------------------------------------------------------
//  Inserts an item in text form into a stream
//	@author Kevin Duffy
// --------------------------------------------------------------------
ostream& operator<<( ostream& p_stream, const Player& p )
{
	// Might be broken after nuking all the stuff from it. Turn it off and fix later. -R
    p_stream << "[NAME]           " << p.m_name << "\n";
    p_stream << "[PASS]           " << p.m_pass << "\n";
	p_stream << "[RANK]           " << GetRankString(p.m_rank) << "\n";
	p_stream << "[LOCATION]       " << p.m_coords.x << " , " << p.m_coords.y << "\n";
	p_stream << "###[RESOURCES]###" << "\n";
	p_stream << "[WOOD]           " << p.m_resourceAmounts[0] << "\n";
	p_stream << "[STONE]          " << p.m_resourceAmounts[1] << "\n";
	p_stream << "[IRON]           " << p.m_resourceAmounts[2] << "\n";
	p_stream << "[GOLD]           " << p.m_resourceAmounts[3] << "\n";
	p_stream << "#####[TOOLS]#####" << "\n";
	p_stream << "[WOOD]           " << p.m_itemLevels[0] << "\n";
	p_stream << "[STONE]          " << p.m_itemLevels[1] << "\n";
	p_stream << "[IRON]           " << p.m_itemLevels[2] << "\n";
	p_stream << "[GOLD]           " << p.m_itemLevels[3] << "\n";
	p_stream << "#####[TITLE]#####" << "\n";
	p_stream << "[WOOD]           " << p.m_firstOfType[0] << "\n";
	p_stream << "[STONE]          " << p.m_firstOfType[1] << "\n";
	p_stream << "[IRON]           " << p.m_firstOfType[2] << "\n";
	p_stream << "[GOLD]           " << p.m_firstOfType[3] << "\n";
	p_stream << "[CORPORATION]    " << p.m_corp << "\n";
	p_stream << "[LEADER]		  " << p.m_corpLeader << "\n";
	p_stream << "[HASSOUL]        " << p.m_hasSoul << "\n";
	p_stream << "[NO.TITLES]      " << p.m_noOfTitles << "\n";
	p_stream << "[TITLE]          " << GetTitleString(p.m_title) << "\n";
	p_stream << "[TITLES]         ";

	Player q = p;
	std::list<PlayerTitle>::iterator itr = q.m_availableTitles.begin();
	for (itr; itr != q.m_availableTitles.end(); ++itr)
	{
		p_stream << GetTitleString(*itr) << " ";
	}
	/*
	//C++11 Kevin RangeLoop vs Itr
	for (auto t : p.m_availableTitles)
	{
		p_stream << t << " ";
	}
	*/
	p_stream << "\n";

	std::map<string, string> temp = q.m_dictionary.GetDictionary();
	p_stream << "[BINDS]          " << temp.size() << "\n";	
	p_stream << "[COMMANDS]       \n";
	//c++11 auto is awesome
	for (auto &c : temp)
	{
		p_stream << "Alias: " << c.first << " Command: " << c.second << "\n";
	}
    return p_stream;
}


// --------------------------------------------------------------------
//  Extracts an item in text form from a stream
//	@author Kevin Duffy
// --------------------------------------------------------------------
istream& operator>>( istream& p_stream, Player& p )
{
	std::string temp;
	std::string temp2;
    p_stream >> temp >> std::ws;
    std::getline( p_stream, p.m_name );
    p_stream >> temp >> p.m_pass;
    p_stream >> temp >> temp;
	p.m_rank = GetRank(temp);
	p_stream >> temp >> p.m_coords.x >> temp >> p.m_coords.y; 
	p_stream >> temp;
	p_stream >> temp >> p.m_resourceAmounts[0];
	p_stream >> temp >> p.m_resourceAmounts[1];
	p_stream >> temp >> p.m_resourceAmounts[2];
	p_stream >> temp >> p.m_resourceAmounts[3];
	p_stream >> temp;
	p_stream >> temp >> p.m_itemLevels[0];
	p_stream >> temp >> p.m_itemLevels[1];
	p_stream >> temp >> p.m_itemLevels[2];
	p_stream >> temp >> p.m_itemLevels[3];
	p_stream >> temp;
	p_stream >> temp >> p.m_firstOfType[0];
	p_stream >> temp >> p.m_firstOfType[1];
	p_stream >> temp >> p.m_firstOfType[2];
	p_stream >> temp >> p.m_firstOfType[3];
	p_stream >> temp >> p.m_corp;
	p_stream >> temp >> p.m_corpLeader;
	p_stream >> temp >> p.m_hasSoul;
	p_stream >> temp >> p.m_noOfTitles;
	
	p_stream >> temp >> temp;
	p.m_title = GetTitle(temp);
    p_stream >> temp;
	for (int i = 0; i < p.m_noOfTitles; i++)
	{
		p_stream >> temp;
		/*if (temp == "-1")
		{
			break;
		}*/
		p.AddTitle(GetSavedTitle(temp));
	}
	int binds;
	p_stream >> temp >> binds;
	p_stream >> temp;
	for (int i = 0; i <= binds; ++i)
	{
		p_stream >> temp >> temp;
		p_stream >> temp2 >> temp2;
		p.m_dictionary.AddCommandPair(temp, temp2);
	}

	
    return p_stream;
}




}   // end namespace SimpleMUD
