// Modified Room database.
// In charge of actually storing the rooms, and then for saving and loading them to and from file.

#ifndef SIMPLEMUDROOMDATABASE_H
#define SIMPLEMUDROOMDATABASE_H


#include <cmath>
#include <string>
#include <map>
#include <set>
#include <stdexcept>

#include "EntityDatabase.h"
#include "Room.h"
#include "DatabasePointer.h"

namespace SimpleMUD
{


// No longer a child of EntityDatabase. Want to be able to use a Vector2 as a key.
// Probably going to be the only case of this so will just do it once here.
// Ronan.
class RoomDatabase
{
protected:
	std::map<BasicLib::vector2, Room, BasicLib::CompareVectors> m_rooms;

public:

	// They need to know the particular way of saving and loading data for these.
	// Should be similar to other room model, but just doing a bit of it manually

    static void LoadTemplates();
    static void LoadData();
    static void SaveData();

	// Adding a room.
	// Adding a new room that it was given.
	void AddRoom(BasicLib::vector2 p_coords, Room p_room);

	// Does a room at coord X exist?
	// Using map.count(Key) to see if the thing is there
	bool RoomExists(BasicLib::vector2 p_coords);

	Room& GetRoom(BasicLib::vector2 p_coords);

};  // end class RoomDatabase



}   // end namespace SimpleMUD

#endif
