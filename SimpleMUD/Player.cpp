// MUD Programming
// Ron Penton
// (C)2003
// Player.cpp - The class defining Players in the SimpleMUD
// 
// 

#include <cmath>
#include <string>
#include <fstream>
#include <vector>

#include "SimpleMUDLogs.h"
#include "Player.h"

namespace SimpleMUD
{

Player::Player()
{
    m_pass = "UNDEFINED";
    m_rank = REGULAR;

    m_connection = 0;
    m_loggedin = false;
    m_active = false;
    m_newbie = true;

    m_experience = 0;
    m_level = 1;

    m_nextattacktime = 0;

    m_baseattributes[STRENGTH] = 1;
    m_baseattributes[HEALTH]   = 1;
    m_baseattributes[AGILITY]  = 1;

    m_statpoints = 18;

    RecalculateStats();
    m_hitpoints = GetAttr( MAXHITPOINTS );

	m_title = PEASANT;
	AddTitle(PEASANT);

	m_coords = vector2(0, 0);
}


int Player::NeedForNextLevel()
{
    // how much more experience to the next level
    return NeedForLevel( m_level + 1 ) - m_experience;
}


bool Player::Train()
{
    if( NeedForNextLevel() <= 0 )
    {
        m_statpoints += 2;
        m_baseattributes[MAXHITPOINTS] += m_level;
        m_level++;
        RecalculateStats();
        return true;
    }
    else
    {
        return false;
    }
}


void Player::RecalculateStats()
{
    m_attributes[MAXHITPOINTS] = 
        10 + ( (int)(m_level * ( GetAttr( HEALTH ) / 1.5 )) );

    m_attributes[HPREGEN] = 
        ( GetAttr( HEALTH ) / 5 ) + m_level;

    m_attributes[ACCURACY] = GetAttr( AGILITY ) * 3;
    m_attributes[DODGING] = GetAttr( AGILITY ) * 3;
    m_attributes[DAMAGEABSORB] = GetAttr( STRENGTH ) / 5;
    m_attributes[STRIKEDAMAGE] = GetAttr( STRENGTH ) / 5;

    // make sure the hitpoints don't overflow if your max goes down
    if( m_hitpoints > GetAttr( MAXHITPOINTS ) )
        m_hitpoints = GetAttr( MAXHITPOINTS );
 
}


void Player::SetBaseAttr( int p_attr, int p_val )
{
    m_baseattributes[p_attr] = p_val;
    RecalculateStats();
}


void Player::AddToBaseAttr( int p_attr, int p_val )
{
    m_baseattributes[p_attr] += p_val;
    RecalculateStats();
}
//	@author Kevin Duffy
void Player::SetTitle(string p_str)
{
	std::list<PlayerTitle>::iterator itr = m_availableTitles.begin();
	while (itr != m_availableTitles.end())
	{
		PlayerTitle& t = *itr;
		string temp = GetTitleString(t);
		if (temp == p_str)
		{
			m_title = t;
			return;
		}		
		++itr;
	}
}
//	@author Kevin Duffy
void Player::AddTitle(PlayerTitle p_val)
{
	bool exists = false;
	std::list<PlayerTitle>::iterator itr = m_availableTitles.begin();
	while (itr != m_availableTitles.end())
	{
		PlayerTitle& t = *itr;
		if (t == p_val)
		{
			exists = true;
		}
		++itr;
	}
	if(!exists)
	m_availableTitles.push_back(p_val);
}

void Player::AddHitpoints( int p_hitpoints )
{
    SetHitpoints( m_hitpoints + p_hitpoints );
}

void Player::SetHitpoints( int p_hitpoints )
{
    m_hitpoints = p_hitpoints;

    if( m_hitpoints < 0 )
        m_hitpoints = 0;
    if( m_hitpoints > GetAttr( MAXHITPOINTS ) )
        m_hitpoints = GetAttr( MAXHITPOINTS );
}


// ------------------------------------------------------------------------
//  This sends a string to the players connection.
// ------------------------------------------------------------------------
void Player::SendString( const std::string& p_string )
{
    using namespace SocketLib;

    if( Conn() == 0 )
    {
        ERRORLOG.Log( "Trying to send string to player " +
                      Name() + " but player is not connected." );
        return;
    }

    // send the string, newline included.
    Conn()->Protocol().SendString( *Conn(), p_string + newline );

    if( Active() )
    {
        PrintStatbar();
    }
}

// ------------------------------------------------------------------------
//  This prints up the players "statbar", ie: his hitpoints.
// ------------------------------------------------------------------------
void Player::PrintStatbar( bool p_update )
{
    using namespace SocketLib;
    using namespace BasicLib;

    // if this is a statusbar update and the user is currently typing something,
    // then do nothing.
    if( p_update && Conn()->Protocol().Buffered() > 0 )
        return;

    string statbar = white + bold + "[";

    int ratio = 100 * HitPoints() / GetAttr( MAXHITPOINTS );

    // color code your hitpoints so that they are red if low,
    // yellow if medium, and green if high.
    if( ratio < 33 )
        statbar += red;
    else if( ratio < 67 )
        statbar += yellow;
    else 
        statbar += green;

    statbar += tostring( HitPoints() ) + white + "/" + 
                tostring( GetAttr( MAXHITPOINTS ) ) + "] ";

    Conn()->Protocol().SendString( *Conn(), clearline + "\r" + statbar + reset );
}



// --------------------------------------------------------------------
//  Inserts an item in text form into a stream
//	@author Kevin Duffy
// --------------------------------------------------------------------
ostream& operator<<( ostream& p_stream, const Player& p )
{
    p_stream << "[NAME]           " << p.m_name << "\n";
    p_stream << "[PASS]           " << p.m_pass << "\n";
	p_stream << "[RANK]           " << GetRankString(p.m_rank) << "\n";
	p_stream << "[TITLE]          " << GetTitleString(p.m_title) << "\n";
    p_stream << "[STATPOINTS]     " << p.m_statpoints << "\n";
    p_stream << "[EXPERIENCE]     " << p.m_experience << "\n";
    p_stream << "[LEVEL]          " << p.m_level << "\n";
    p_stream << "[HITPOINTS]      " << p.m_hitpoints << "\n";
    p_stream << "[NEXTATTACKTIME] "; insert( p_stream, p.m_nextattacktime );
    p_stream << "\n";
    p_stream << p.m_baseattributes;

	p_stream << "[TITLES]         ";
	
		Player q = p;
		std::list<PlayerTitle>::iterator itr = q.m_availableTitles.begin();
		for (itr; itr != q.m_availableTitles.end(); ++itr)
		{
			p_stream << GetTitleString(*itr) << " ";
		}

		p_stream << "-1";
    p_stream << "\n";


    return p_stream;
}


// --------------------------------------------------------------------
//  Extracts an item in text form from a stream
//	@author Kevin Duffy
// --------------------------------------------------------------------
istream& operator>>( istream& p_stream, Player& p )
{
    std::string temp;
    p_stream >> temp >> std::ws;
    std::getline( p_stream, p.m_name );
    p_stream >> temp >> p.m_pass;
    p_stream >> temp >> temp;
	p.m_rank = GetRank(temp);
	p_stream >> temp >> temp;
	p.m_title = GetTitle(temp);
    p_stream >> temp >> p.m_statpoints;
    p_stream >> temp >> p.m_experience;
    p_stream >> temp >> p.m_level;
    p_stream >> temp >> p.m_hitpoints;
    p_stream >> temp; extract( p_stream, p.m_nextattacktime );
    p_stream >> p.m_baseattributes;

    p_stream >> temp;
	for (int i = 0; i < 8; i++)
	{
		p_stream >> temp;
		if (temp == "-1")
		{
			break;
		}

		p.AddTitle(GetTitle(temp));
	}
  
	//	p_stream << "\n";
	//}
	//for (int i = 0; i < p.MaxItems(); i++)
	//{
	//	p_stream >> p.m_inventory[i];
	//	if (p.m_inventory[i] != 0)
	//		p.m_items++;
	//}

    p.RecalculateStats();

    return p_stream;
}




}   // end namespace SimpleMUD
