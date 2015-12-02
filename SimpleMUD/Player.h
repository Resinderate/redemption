// MUD Programming
// Ron Penton
// (C)2003
// Player.h - The class defining Players in the SimpleMUD
// 
// 


#ifndef PLAYER_H
#define PLAYER_H

#include <math.h>
#include <string>
#include <array>
#include "../SocketLib/SocketLib.h"
#include "../BasicLib/BasicLib.h"

#include "Entity.h"
#include "Attributes.h"
#include "DatabasePointer.h"
#include "World.h"
#include "CommandDictionary.h"
class Room;

using SocketLib::Connection;
using SocketLib::Telnet;
using namespace BasicLib;
using std::ostream;
using std::istream;
using std::string;

namespace SimpleMUD
{

class Player : public Entity
{
public:

    Player();

    // ------------------------------------------------------------------------
    //  Level Functions
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    //  Attribute Functions
    // ------------------------------------------------------------------------
	std::list<vector2> AdjacentRooms();
	inline std::shared_ptr<Room>& CurrentRoom() { return World::GetRoom(m_coords); }
	inline vector2& Coords() { return m_coords; }

	inline PlayerTitle& GetPlayerTitle() { return m_title; }
	inline std::list<PlayerTitle>& Titles() { return m_availableTitles; }
	void SetTitle(string p_str);
	void AddTitle(PlayerTitle p_val);

	inline std::array<resource, NumResourceType>& GetResources() { return m_resourceAmounts; }
	inline std::array<int, NumResourceType>& GetItemLevels() { return m_itemLevels; }

    // ------------------------------------------------------------------------
    //  non-savable accessors
    // ------------------------------------------------------------------------
    inline string& Password()               { return m_pass; }
    inline PlayerRank& Rank()               { return m_rank; }
    inline Connection<Telnet>*& Conn()      { return m_connection; }
    inline bool& LoggedIn()                 { return m_loggedin; }
    inline bool& Active()                   { return m_active; }
    inline bool& Newbie()                   { return m_newbie; }
	inline CommandDictionary& GetDict()		{ return m_dictionary; }
	inline bool& HasSoul()					{ return m_hasSoul; }

    // ------------------------------------------------------------------------
    //  Communications Functions
    // ------------------------------------------------------------------------
    void SendString( const string& p_string );
    void PrintStatbar( bool p_update = false );

    // ------------------------------------------------------------------------
    //  Stream Functions
    // ------------------------------------------------------------------------
    friend ostream& operator<<( ostream& p_stream, const Player& p );
    friend istream& operator>>( istream& p_stream, Player& p );


protected:
    // -----------------------------------------
    //  Player Information
    // -----------------------------------------
    string m_pass;
	PlayerRank m_rank;

    // -----------------------------------------
    //  Player attributes
    // -----------------------------------------
	// No longer has a room. Just Coords.
	vector2 m_coords;

	// All an array of length 4, to correspond to the different resources.
	std::array<resource, NumResourceType> m_resourceAmounts;
	std::array<int, NumResourceType> m_itemLevels;

	PlayerTitle m_title;
	int m_noOfTitles;
	std::list<PlayerTitle> m_availableTitles;

	string m_corp;

	CommandDictionary m_dictionary;

	bool m_hasSoul;

    // -----------------------------------------
    //  Non-savable info
    // -----------------------------------------
    Connection<Telnet>* m_connection;
    bool m_loggedin;
    bool m_active;
    bool m_newbie;

};  // end class Player



ostream& operator<<( ostream& p_stream, const Player& p );
istream& operator>>( istream& p_stream, Player& p );

// --------------------------------------------------------------------
//  functor that determines if a player is active
// --------------------------------------------------------------------
struct playeractive
{ 
    inline bool operator() ( Player& p_player )
    {
        return p_player.Active();
    }

    inline bool operator() ( Player* p_player )
    {
        return p_player != 0 && p_player->Active();
    }
};

// --------------------------------------------------------------------
//  functor that determines if a player is logged in
// --------------------------------------------------------------------
struct playerloggedin
{ 
    inline bool operator() ( Player& p_player )
    {
        return p_player.LoggedIn();
    }

    inline bool operator() ( Player* p_player )
    {
        return p_player != 0 && p_player->LoggedIn();
    }

};


// --------------------------------------------------------------------
//  functor that sends a string to a player.
// --------------------------------------------------------------------
struct playersend
{
    const string& m_msg;

    playersend( const string& p_msg )
        : m_msg( p_msg ) { /* do nothing */ }

    void operator() ( Player& p )
    {
        p.SendString( m_msg );
    }

    void operator() ( Player* p )
    {
        if( p != 0 )
            p->SendString( m_msg );
    }
};

}   // end namespace SimpleMUD


#endif
