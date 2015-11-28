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
#include "Item.h"
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

    inline string& Description()    { return m_description; }

	inline list<player>& Players()  { return m_players; }

    void AddPlayer( player p_player );
    void RemovePlayer( player p_player );

    void LoadTemplate( istream& p_stream );
    void LoadData( istream& p_stream );
    void SaveData( ostream& p_stream );


protected:
    // -----------------------------------------
    //  template information
    // -----------------------------------------
	string m_name;
    string m_description;
	//BasicLib::vector2 m_coords;



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
