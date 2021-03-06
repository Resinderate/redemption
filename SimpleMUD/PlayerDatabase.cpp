// MUD Programming
// Ron Penton
// (C)2003
// PlayerDatabase.cpp - The class that stores all of the players within the MUD
// 
// 

#include <fstream>
#include "../BasicLib/BasicLib.h"
#include "SimpleMUDLogs.h"
#include "PlayerDatabase.h"

using BasicLib::LowerCase;
using BasicLib::tostring;
using std::string;
using std::ifstream;
using std::ofstream;


namespace SimpleMUD
{

// declare the static variables of the player database.
std::map<entityid, Player> EntityDatabase<Player>::m_map;


void PlayerDatabase::LoadPlayer( string p_name )
{
    entityid id;
    string temp;
    p_name = PlayerFileName( p_name );      // create the proper filename
    ifstream file( p_name.c_str() );        // open the file
    file >> temp >> id;                     // load the ID
    m_map[id].ID() = id;
    file >> m_map[id] >> std::ws;           // load the player from the file
    USERLOG.Log( "Loaded Player: " + m_map[id].Name() );
}

void PlayerDatabase::SavePlayer( entityid p_player )
{
    std::map<entityid, Player>::iterator itr = m_map.find( p_player );
    if( itr == m_map.end() )
        return;

    std::string name = PlayerFileName( itr->second.Name() );
    ofstream file( name.c_str() );
    
    file << "[ID]             " << p_player << "\n";
    file << itr->second;
}

std::list<Player> PlayerDatabase::GetAllPlayers()
{
	std::list<Player> players;
	for (auto kv : m_map)
	{
		players.push_back(kv.second);
	}
	return players;
}


bool PlayerDatabase::Load() 
{
    ifstream file( "players/players.txt" );
    string name;

    while( file.good() )           // while there are players
    {
        file >> name >> std::ws;   // load in the player name
		if (name != "")
			LoadPlayer( name );        // call the LoadPlayer helper function
    }
    return true;
}


bool PlayerDatabase::Save()
{
    ofstream file( "players/players.txt" );
    iterator itr = begin();

    while( itr != end() )
    {
        file << itr->Name() << "\n";
        SavePlayer( itr->ID() );        
        ++itr;
    }

    return true;
}


bool PlayerDatabase::AddPlayer( Player& p_player )
{
    if( has( p_player.ID() ) )
        return false;
    if( hasfull( p_player.Name() ) )
        return false;

    // insert the player into the map
    m_map[p_player.ID()] = p_player;

    // add the players name to the players.txt file
    std::ofstream file( "players/players.txt", std::ios::app );
    file << p_player.Name() << "\n";


    // write the initial player out to disk
    SavePlayer( p_player.ID() );

    return true;
}

std::list<player> PlayerDatabase::CorpMembers(std::string p_guildName)
{
	std::list<player> players;
	for (auto &kv : m_map)
	{
		if(kv.second.CorpName() == p_guildName)
			players.push_back(kv.second.ID());
	}
	return players;
}

std::set<std::string> PlayerDatabase::AllCorpNames()
{
	std::set<std::string> corpNames;
	for (auto &p : GetAllPlayers())
	{
		corpNames.insert(p.CorpName());
	}
	return corpNames;
}
    

void PlayerDatabase::Logout( entityid p_player )
{
    Player& p = get( p_player );

    USERLOG.Log(  
        SocketLib::GetIPString( p.Conn()->GetRemoteAddress() ) + 
        " - User " + p.Name() + " logged off." );

    p.Conn() = 0;
    p.LoggedIn() = false;
    p.Active() = false;

    // make sure the player is saved to disk
    SavePlayer( p_player );
}




}   // end namespace SimpleMUD
