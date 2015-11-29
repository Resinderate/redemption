// MUD Programming
// Ron Penton
// (C)2003
// PlayerDatabase.cpp - The class that stores all of the players within the MUD
// 
// 

#include <fstream>
#include <vector>
#include "../BasicLib/BasicLib.h"
#include "SimpleMUDLogs.h"
#include "RoomDatabase.h"

using BasicLib::LowerCase;
using BasicLib::tostring;
using std::string;
using std::ifstream;
using std::ofstream;


namespace SimpleMUD
{

	// declare the static vector of the room database.
	//std::vector<Room> EntityDatabaseVector<Room>::m_vector;
	std::unordered_map<BasicLib::vector2, Room> RoomDatabase::m_rooms;

	void RoomDatabase::AddRoom(vector2 p_coords, Room p_room)
	{
		m_rooms.insert(std::pair<vector2, Room>( p_coords, p_room ));
	}

	bool RoomDatabase::RoomExists(vector2 p_coords)
	{
		if (m_rooms.count(p_coords) > 0)
			return true;
		else
			return false;
	}

	Room & SimpleMUD::RoomDatabase::GetRoom(BasicLib::vector2 p_coords)
	{
		// Could throw an exception if the item doesn't exist.
		try
		{
			return m_rooms.at(p_coords);
		}
		catch (std::exception)
		{
			ERRORLOG.Log("Error finding room in map.");
		}

		
	}

	void RoomDatabase::LoadTemplates()
	{
		// Load Tempaltes.
		/*
		std::ifstream file( "maps/default.map" );
		entityid id;
		std::string temp;

		while( file.good() )
		{
			// read the ID from disk
			file >> temp >> id;

			// make sure there's enough room for the room
			if( m_vector.size() <= id )
				m_vector.resize( id + 1 );

			m_vector[id].ID() = id;
			m_vector[id].LoadTemplate( file );     
			file >> std::ws;
		}
		*/
	}

	void RoomDatabase::LoadData()
	{
		// Load Data.
		/*
		std::ifstream file( "maps/default.data" );

		string temp;
		entityid roomid;

		while( file.good() )
		{
			// load in the room id
			file >> temp >> roomid;

			// load the entry
			m_vector[roomid].LoadData( file );
			file >> std::ws;
		}
		*/
	}

	void RoomDatabase::SaveData()
	{
		// Save Data.
	}


}   // end namespace SimpleMUD
