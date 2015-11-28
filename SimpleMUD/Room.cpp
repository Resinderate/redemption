// MUD Programming
// Ron Penton
// (C)2003
// Room.cpp - The class defining Rooms in the SimpleMUD
// 
// 



#include <string>
#include <iostream>

#include "Room.h"
#include "Enemy.h"
#include "../BasicLib/BasicLib.h"

using std::ostream;
using std::istream;
using BasicLib::extract;

// Restructuring the room hierarchy - Ronan.
namespace SimpleMUD
{

Room::Room()
{
	// Might be a type instead.
	m_name= "UNDEFINED";
    m_description = "UNDEFINED";

	//m_coords = BasicLib::vector2(0, 0);
}

void Room::AddPlayer( player p_player )
{
    m_players.push_back( p_player );
}

void Room::RemovePlayer( player p_player )
{
    m_players.erase( std::find( m_players.begin(), 
                                m_players.end(), 
                                (entityid)p_player ) );
}

void Room::LoadTemplate( istream& p_stream )
{
    string temp;

    p_stream >> temp >> std::ws;    std::getline( p_stream, m_name );
    p_stream >> temp >> std::ws;    std::getline( p_stream, m_description );
}

void Room::LoadData( istream& p_stream )
{
    string temp;
    p_stream >> temp;
}

void Room::SaveData( ostream& p_stream )
{

}

}   // end namespace SimpleMUD

