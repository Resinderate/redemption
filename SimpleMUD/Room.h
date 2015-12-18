// MUD Programming
// Ron Penton
// (C)2003
// Room.h - The class defining Rooms in the SimpleMUD
// 
// 


#ifndef SIMPLEMUDROOM_H
#define SIMPLEMUDROOM_H

#include <string>
#include <iostream>
#include <list>
#include "../BasicLib/BasicLib.h"

#include "Entity.h"
#include "Attributes.h"
#include "DatabasePointer.h"

using std::ostream;
using std::istream;
using std::list;

namespace SimpleMUD
{


// --------------------------------------------------------------------
//  Class that stores information about rooms
// --------------------------------------------------------------------
class Room : public Entity
{
public:
    Room();
	virtual ~Room();
	Room(string p_name, string p_desc, RoomBaseType p_baseType, BasicLib::vector2 p_coords);
	inline string& Name()			{ return m_name; }
    inline string& Description()    { return m_description; }

    inline list<player>& Players()  { return m_players; }

    void AddPlayer( player p_player );
    void RemovePlayer( player p_player );

    void LoadTemplate( istream& p_stream );
    void LoadData( istream& p_stream );
    void SaveData( ostream& p_stream );

	inline BasicLib::vector2 GetCoords() { return m_coords; }
	inline RoomBaseType GetBaseType() { return m_baseType;  }

protected:
    // -----------------------------------------
    //  template information
    // -----------------------------------------
	
	// Might be some other symantic for this.
	string m_name;
    string m_description;
	BasicLib::vector2 m_coords;
	RoomBaseType m_baseType;	

    // -----------------------------------------
    //  volatile data (save to disk)
    // -----------------------------------------


    // -----------------------------------------
    //  volatile data (do not save to disk)
    // -----------------------------------------
    list<player> m_players;


};  // end class Room

}   // end namespace SimpleMUD


#endif
