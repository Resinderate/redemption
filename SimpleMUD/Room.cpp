// MUD Programming
// Ron Penton
// (C)2003
// Room.cpp - The class defining Rooms in the SimpleMUD
// 
// 

#include <string>
#include <iostream>

#include "Room.h"
#include "../BasicLib/BasicLib.h"

using std::ostream;
using std::istream;
using BasicLib::extract;

namespace SimpleMUD
{

Room::Room()
{
	m_name = "Default Room Name";
    m_description = "Default Room Desc";
	m_coords = BasicLib::vector2();
	// Should always set this correctly for each room.
	m_baseType = RoomBaseType::DEFAULT;
	m_owner = "";
	m_boughtTimes = 0;
}

Room::Room(string p_name, string p_desc, RoomBaseType p_baseType, BasicLib::vector2 p_coords, string p_owner, int p_boughtTimes) :
	m_name(p_name),
	m_description(p_desc),
	m_baseType(p_baseType),
	m_coords(p_coords),
	m_owner(p_owner),
	m_boughtTimes(p_boughtTimes)
{
}

Room::~Room()
{
}

void Room::AddPlayer( player p_player )
{
    m_players.push_back( p_player );
}

void Room::RemovePlayer( player p_player )
{
	auto pIt = std::find(m_players.begin(), m_players.end(), (entityid)p_player);
    m_players.erase(pIt);
}


void Room::LoadTemplate( istream& p_stream )
{
	/*
    string temp;

    p_stream >> temp >> std::ws;    std::getline( p_stream, m_name );
	p_stream >> temp >> std::ws;    std::getline(p_stream, m_description);
	*/
}

void Room::LoadData( istream& p_stream )
{
    //string temp;
    //p_stream >> temp;

    //// clear all the existing items, then load in all the new items
    //m_items.clear();        
    //entityid last;
    //while( extract( p_stream, last ) != 0 )
    //    m_items.push_back( last );

}

void Room::SaveData( ostream& p_stream )
{
 /*   p_stream << "[ITEMS] ";
    
    std::list<item>::iterator itr = m_items.begin();
    while( itr != m_items.end() )
    {
        p_stream << *itr << " ";
        ++itr;
    }

    p_stream << "0\n";

    p_stream << "[MONEY] " << m_money << "\n";*/
}

BasicLib::resource Room::CurrerntCost()
{
	BasicLib::resource baseCost = 100;
	BasicLib::resource cost = baseCost;

	for (int i = 0; i < m_boughtTimes; i++)
	{
		cost *= 1.5f;
	}

	return BasicLib::resource(cost);
}



}   // end namespace SimpleMUD

