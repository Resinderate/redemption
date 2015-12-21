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
#include "CollectingRoom.h"
#include "SpecialRoom.h"
#include "World.h"

using BasicLib::LowerCase;
using BasicLib::tostring;
using std::string;
using std::ifstream;
using std::ofstream;


namespace SimpleMUD
{

	// declare the static vector of the room database
	std::unordered_map<BasicLib::vector2, std::shared_ptr<Room>> RoomDatabase::m_rooms;

	void RoomDatabase::AddRoom(vector2 p_coords, std::shared_ptr<Room> p_room)
	{
		m_rooms.insert(std::pair<vector2, std::shared_ptr<Room>>( p_coords, p_room ));
	}

	bool RoomDatabase::RoomExists(vector2 p_coords)
	{
		if (m_rooms.count(p_coords) > 0)
			return true;
		else
			return false;
	}

	std::shared_ptr<Room>& SimpleMUD::RoomDatabase::GetRoom(BasicLib::vector2 p_coords)
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
	/*void RoomDatabase::Load()
	{
		ifstream file("players/players.txt");
		string type;

		
	}*/

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
		
		std::ifstream file( "maps/default.data" );
		string str;
		RoomBaseType tempt;
		while( file.good() )
		{
			// load the entry
			file >> str >> str;
			RoomBaseType t = GetRoomBaseType(str);
			if (t == COLLECTING)
			{
				string temp, name, description, owner;
				int bought;
				vector2 coords;
				ResourceSize resourceSize;
				ResourceType resourceType;

				file >> temp >> name;
				file >> temp >> coords.x >> temp >> coords.y;
				file >> temp >> temp;
				resourceType = GetResourceTypeEnum(temp);
				file >> temp >> temp;
				resourceSize = GetResourceSizeEnum(temp);
				file >> temp >> owner;
				file >> temp >> bought;

				description = GetResourceTypeString(resourceType) + " :: " + GetResourceSizeString(resourceSize) +
					"\r\nCoords: " + std::to_string(coords.x) + ", " + std::to_string(coords.y);
				
				AddRoom(coords, std::unique_ptr<Room>(new CollectingRoom(name, description, t, coords, resourceType, resourceSize, owner, bought)));
			}
			if (t == SPECIAL)
			{
				string temp, name, description;
				vector2 coords;
				RoomType roomType;

				file >> temp >> name;
				file >> temp >> coords.x >> temp >> coords.y;
				file >> temp;
				std::getline(file, temp);
				roomType = GetRoomType(temp);

				description = GetRoomTypeString(roomType) +
					"\r\nCoords: " + std::to_string(coords.x) + ", " + std::to_string(coords.y);

				AddRoom(coords, std::unique_ptr<Room>(new SpecialRoom(name, description, t, coords, roomType)));
			}
			//AddRoom(temp.GetCoords(), temp);
			file >> std::ws;
		}
		
	}

	void RoomDatabase::SaveData()
	{
		std::ofstream file("maps/default.data");

		auto itr = m_rooms.begin();

		while (itr != m_rooms.end())
		{
			RoomBaseType t = itr->second->GetBaseType();
			if (t == COLLECTING)
			{
				file << "[ROOMBASETYPE] " << GetRoomBaseTypeString(itr->second->GetBaseType()) << "\n";
				std::shared_ptr<Room>& r = itr->second;
				CollectingRoom* cRoom = dynamic_cast<CollectingRoom*>(r.get());
				
				file << "[NAME] " << itr->second->Name() << "\n";
				file << "[COORDINATES] " << itr->second->GetCoords().x << " , " << itr->second->GetCoords().y << "\n";
				file << "[RESOURCETYPE] " << GetResourceTypeString(cRoom->GetResourceType()) << "\n";
				file << "[RESOURCESIZE]	" << GetResourceSizeString(cRoom->GetResourceSize()) << "\n";
				file << "[OWNER] " << cRoom->Owner() << "\n";
				file << "[TIMESBOUGHT] " << cRoom->BoughtTimes() << "\n";
			}
			else if (t == SPECIAL)
			{
				file << "[ROOMBASETYPE] " << GetRoomBaseTypeString(itr->second->GetBaseType()) << "\n";
				std::shared_ptr<Room>& r = itr->second;
				SpecialRoom* sRoom = dynamic_cast<SpecialRoom*>(r.get());

				file << "[NAME] " << itr->second->Name() << "\n";
				file << "[COORDINATES] " << itr->second->GetCoords().x << " , " << itr->second->GetCoords().y << "\n";
				file << "[ROOMTYPE] " << GetRoomTypeString(sRoom->GetRoomType()) << "\n";
			}
			else
			{
			}
			file << "\n";
			++itr;
		}
	}


}   // end namespace SimpleMUD
