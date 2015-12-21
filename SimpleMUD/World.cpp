// Implementation of World class.
// Ronan

#include <string>
#include <iostream>

#include "World.h"

using std::ostream;
using std::istream;
using BasicLib::extract;

namespace SimpleMUD
{
	RoomDatabase World::m_rooms;

	World::World()
	{

	}

	std::shared_ptr<Room>&World::GetRoom(BasicLib::vector2 p_coords)
	{
		// While there is not room generated.
		// Shouldn't really be able to get to a room that hasn't been generated, but as a fail safe.
		if (!m_rooms.RoomExists(p_coords))
		{
			GenerateNewRoom(p_coords);
		}
		return m_rooms.GetRoom(p_coords);
	}

	BasicLib::vector2 World::ChangeRoom(BasicLib::vector2 p_coords, BasicLib::vector2 p_move)
	{
		BasicLib::vector2 newLocation = BasicLib::vector2(p_coords.x + p_move.x, p_coords.y + p_move.y);

		if (!m_rooms.RoomExists(newLocation))
		{
			GenerateNewRoom(newLocation);
		}

		return newLocation;
	}

	bool World::RoomExists(BasicLib::vector2 p_coords)
	{
		return m_rooms.RoomExists(p_coords);
	}

	void World::GenerateNewRoom(BasicLib::vector2 p_coords)
	{
		float distanceFromOrigin = p_coords.length();
		string randomName = PlaceNameGenerator::GetRandomPlaceName();

		// Clean up dupped code if hard coding.
		if (p_coords.x == -1 && p_coords.y == 0)
		{
			string name = randomName;
			string desc = string("Trading Post") + 
				"\r\nCoords: " + std::to_string(p_coords.x) + ", " + std::to_string(p_coords.y);
			RoomBaseType roomBaseType = RoomBaseType::SPECIAL;
			RoomType roomType = RoomType::TRADING;
			m_rooms.AddRoom(p_coords, std::unique_ptr<Room>(new SpecialRoom(name, desc, roomBaseType, p_coords, roomType)));
		}
		else if (p_coords.x == -5 && p_coords.y == -5)
		{
			string name = randomName;
			string desc = string("Devil Room") +
				"\r\nCoords: " + std::to_string(p_coords.x) + ", " + std::to_string(p_coords.y);
			RoomBaseType roomBaseType = RoomBaseType::SPECIAL;
			RoomType roomType = RoomType::DEVIL;
			m_rooms.AddRoom(p_coords, std::unique_ptr<Room>(new SpecialRoom(name, desc, roomBaseType, p_coords, roomType)));
		}
		else if (p_coords.x == 0 && p_coords.y == 1)
		{
			string name = randomName;
			string desc = string("Workshop") +
				"\r\nCoords: " + std::to_string(p_coords.x) + ", " + std::to_string(p_coords.y);
			RoomBaseType roomBaseType = RoomBaseType::SPECIAL;
			RoomType roomType = RoomType::WORKSHOP;
			m_rooms.AddRoom(p_coords, std::unique_ptr<Room>(new SpecialRoom(name, desc, roomBaseType, p_coords, roomType)));
		}
		else if (p_coords.x == 0 && p_coords.y == 1)
		{
			string name = randomName;
			string desc = string("Corporation House") +
				"\r\nCoords: " + std::to_string(p_coords.x) + ", " + std::to_string(p_coords.y);
			RoomBaseType roomBaseType = RoomBaseType::SPECIAL;
			RoomType roomType = RoomType::CORP;
			m_rooms.AddRoom(p_coords, std::unique_ptr<Room>(new SpecialRoom(name, desc, roomBaseType, p_coords, roomType)));
		}
		else if (p_coords.x == 5 && p_coords.y == 5)
		{
			string name = randomName;
			string desc = string("Assassin Room") +
				"\r\nCoords: " + std::to_string(p_coords.x) + ", " + std::to_string(p_coords.y);
			RoomBaseType roomBaseType = RoomBaseType::SPECIAL;
			RoomType roomType = RoomType::ASSASSIN;
			m_rooms.AddRoom(p_coords, std::unique_ptr<Room>(new SpecialRoom(name, desc, roomBaseType, p_coords, roomType)));
		}
		else
		{
			RoomBaseType roomType = RoomBaseType::COLLECTING;

			// Random Room Type.
			int typeIndex = -1;

			// Random between 0.0f and 1.0f
			float random = static_cast<float> (rand()) / static_cast<float> (RAND_MAX);

			// Pick an index based on the weights.
			int size = sizeof(TYPEWEIGHTS) / sizeof(TYPEWEIGHTS[0]);
			for (int i = 0; i < size; i++)
			{
				random -= TYPEWEIGHTS[i];
				if (random <= 0.0f)
				{
					typeIndex = i;
					break;
				}
			}

			// Assigning Type for the room.
			ResourceType resourceType = static_cast<ResourceType>(typeIndex);

			// Random Room Size.
			int sizeIndex = -1;
			random = static_cast<float> (rand()) / static_cast<float> (RAND_MAX);

			size = sizeof(SIZEWEIGHTS) / sizeof(SIZEWEIGHTS[0]);
			for (int i = 0; i < size; i++)
			{
				random -= SIZEWEIGHTS[i];
				if (random <= 0.0f)
				{
					sizeIndex = i;
					break;
				}
			}

			// Size
			ResourceSize resourceSize = static_cast<ResourceSize>(sizeIndex);

			string name = randomName;
			string desc = ResourceTypeStrings[typeIndex] + " :: " + ResourceSizeStrings[sizeIndex] +
				"\r\nCoords: " + std::to_string(p_coords.x) + ", " + std::to_string(p_coords.y);

			m_rooms.AddRoom(p_coords, std::unique_ptr<Room>(new CollectingRoom(name, desc, roomType, p_coords, resourceType, resourceSize, OWNERNONE, 0)));
		}
	}

}   // end namespace SimpleMUD

