// MUD Programming
// Ron Penton
// (C)2003
// Attributes.h - This file defines all player attributes.
// 
// 


#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include <vector>


using std::string;
using std::ostream;
using std::istream;

namespace SimpleMUD
{

// ======================================
//  GENERIC FUNCTIONS
// ======================================

// ----------------------------------------------------------------------------
//  A function to convert a string into an enumeration. It takes three
//  template parameters: the type of the enumeration, the size of the enumeration,
//  and an array of strings holding the enumeration names
// ----------------------------------------------------------------------------
template< typename enumeration, int size >
inline enumeration StrToEnum( string p_str, const string* strs )
{
    p_str = BasicLib::UpperCase( p_str );

    for( int i = 0; i < size; i++ )
    {
        if( strs[i] == p_str )
            return (enumeration)(i);
    }

    return (enumeration)(0);
}

// ----------------------------------------------------------------------------
//  A function to convert an enumeration into a string. It takes three
//  template parameters: the type of the enumeration, the size of the enumeration,
//  and an array of strings holding the enumeration names
// ----------------------------------------------------------------------------
template< typename enumeration >
inline string EnumToStr( enumeration p_enum, const string* strs )
{
    return strs[p_enum];
}

const std::string CORPNONE = "None";
const std::string OWNERNONE = "None";

// ======================================
//  PLAYER TITLES
// ======================================

// --------------------------------------------------------------------
//  An enumeration defining the various player types and accompanying
//  functions
//	@author Kevin Duffy
// --------------------------------------------------------------------
enum PlayerTitle
{
	PEASANT,
	GOLDLEADER,
	IRONLEADER,
	STONELEADER,
	WOODLEADER,
	THEREDEEMED,
	CEO,
	GOTWOOD,
	STONECOLD,
	IRONMAIDEN,
	GOLDDIGGER
};

const int NUMPLAYERTITLETYPES = 11;

const string PLAYERTITLESTRINGS[NUMPLAYERTITLETYPES] =
{
	"Peasant",
	"GoldLeader",
	"IronLeader",
	"StoneLeader",
	"WoodLeader",
	"TheRedeemed",
	"CEO",
	"GotWood",
	"StoneCold",
	"IronMaiden",
	"GoldDigger"
};

const string PLAYERTITLESAVE[NUMPLAYERTITLETYPES] =
{
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"10"
};


inline PlayerTitle GetTitle(string p_str)
{
	return StrToEnum<PlayerTitle, NUMPLAYERTITLETYPES>(p_str, PLAYERTITLESTRINGS);
}
inline PlayerTitle GetSavedTitle(string p_str)
{
	return StrToEnum<PlayerTitle, NUMPLAYERTITLETYPES>(p_str, PLAYERTITLESAVE);
}
inline string GetTitleString(PlayerTitle p_enum)
{
	return PLAYERTITLESTRINGS[p_enum];
}




// ======================================
//  PLAYER RANKS
// ======================================

// --------------------------------------------------------------------
//  An enumeration defining the various player types and accompanying
//  functions
// --------------------------------------------------------------------
enum PlayerRank
{
    REGULAR,
    GOD,
    ADMIN
};

const int NUMPLAYERRANKTYPES = 3;

const string PLAYERRANKSTRINGS[NUMPLAYERRANKTYPES] =
{
    "REGULAR",
    "GOD",
    "ADMIN"
};


inline PlayerRank GetRank( string p_str )
{
    return StrToEnum<PlayerRank, NUMPLAYERRANKTYPES>( p_str, PLAYERRANKSTRINGS );
}
inline string GetRankString( PlayerRank p_enum )
{
    return EnumToStr<PlayerRank>( p_enum, PLAYERRANKSTRINGS );
}



// ======================================
//  ROOM TYPES
// ======================================


enum RoomBaseType
{
	DEFAULT,
	COLLECTING,
	SPECIAL
};

const int NUMROOMBASETYPES = 3;

const string NUMROOMBASETYPESTRINGS[NUMROOMBASETYPES] =
{
	"DEFAULT",
	"COLLECTING",
	"SPECIAL"
};

inline RoomBaseType GetRoomBaseType(string p_str)
{
	return StrToEnum<RoomBaseType, NUMROOMBASETYPES>(p_str, NUMROOMBASETYPESTRINGS);
}
inline string GetRoomBaseTypeString(RoomBaseType p_enum)
{
	return EnumToStr<RoomBaseType>(p_enum, NUMROOMBASETYPESTRINGS);
}

// --------------------------------------------------------------------
//  An enumeration defining the various room types and accompanying
//  functions
// --------------------------------------------------------------------
enum RoomType
{
	TRADING,
	WORKSHOP,
	DEVIL,
	CORP,
	ASSASSIN
	// Add more types of rooms as needed.
};

// Might need this stuff.

const int NUMROOMTYPES = 5;

const string ROOMTYPESTRINGS[NUMROOMTYPES] =
{
	"Trading Post",
	"Workshop",
	"Devil Room",
	"Corporation House",
	"Assassin Room"
};


inline RoomType GetRoomType( string p_str )
{
    return StrToEnum<RoomType, NUMROOMTYPES>( p_str, ROOMTYPESTRINGS );
}
inline string GetRoomTypeString( RoomType p_enum )
{
    return EnumToStr<RoomType>( p_enum, ROOMTYPESTRINGS );
}


// Room Type and Size Weights
// Ronan

// Type
const float WOODWEIGHT = 0.4f;
const float STONEWEIGHT = 0.3f;
const float IRONWEIGHT = 0.2f;
const float GOLDWEIGHT= 0.1f;

const float TYPEWEIGHTS[4] = {WOODWEIGHT, STONEWEIGHT, IRONWEIGHT, GOLDWEIGHT};
// Size
const float SMALLWEIGHT = 0.5f;
const float MEDIUMWEIGHT = 0.3f;
const float LARGEWEIGHT = 0.2f;

const float SIZEWEIGHTS[4] = { SMALLWEIGHT, MEDIUMWEIGHT, LARGEWEIGHT };

// All weights should add up to 1.0f
const float TOTALWEIGHT = 1.0f;




// ======================================
//  DIRECTIONS
// ======================================

// --------------------------------------------------------------------
//  An enumeration defining the various room directions and accompanying
//  functions
// --------------------------------------------------------------------
enum Direction
{
    NORTH,
    EAST,
    SOUTH,
    WEST
};

const int NUMDIRECTIONS = 4;

inline int OppositeDirection( int p_dir )
{
    return (p_dir + 2) % 4;
}

const string DIRECTIONSTRINGS[NUMDIRECTIONS] =
{
    "NORTH",
    "EAST",
    "SOUTH",
    "WEST"
};


inline Direction GetDirection( string p_str )
{
    return StrToEnum<Direction, NUMDIRECTIONS>( p_str, DIRECTIONSTRINGS );
}
inline string GetDirectionString( Direction p_enum )
{
    return EnumToStr<Direction>( p_enum, DIRECTIONSTRINGS );
}

// ======================================
//  Resources
// ======================================

enum ResourceType
{
	WOOD,
	STONE,
	IRON,
	GOLD
};
const int NumResourceType = 4;
const int WorldSize = 500;
const int maxCorpLength = 20;

const string ResourceTypeStrings[NumResourceType] =
{
	"WOOD",
	"STONE",
	"IRON",
	"GOLD"
};

inline ResourceType GetResourceTypeEnum(string p_str)
{
	return StrToEnum<ResourceType, NumResourceType>(p_str, ResourceTypeStrings);
}
inline string GetResourceTypeString(ResourceType p_enum)
{
	return EnumToStr<ResourceType>(p_enum, ResourceTypeStrings);
}

enum ResourceSize
{
	SMALL,
	MEDIUM,
	LARGE
};
const int NumResourceSize = 3;

const string ResourceSizeStrings[NumResourceSize] =
{
	"SMALL",
	"MEDIUM",
	"LARGE"
};

inline ResourceSize GetResourceSizeEnum(string p_str)
{
	return StrToEnum<ResourceSize, NumResourceSize>(p_str, ResourceSizeStrings);
}
inline string GetResourceSizeString(ResourceSize p_enum)
{
	return EnumToStr<ResourceSize>(p_enum, ResourceSizeStrings);
}

}   // end namespace SimpleMUD


// Stuff needed for a better rebind system.

const std::vector<string> BaseCommands = 
{
	"say",
	"shout",
	"corp",
	"global",
	"whisper",
	"help",
	"exit",
	"quit",
	"stats",
	"time",
	"who",
	"look",
	"north",
	"south",
	"west",
	"east",
	"titles",
	"change",
	"rebind",
	"collect",
	"trade",
	"interact",
	"invite",
	"leavecorp",
	"leaderboards",
	"report",
	"warp",
	"kick",
	"mute",
	"announce",
	"promote",
	"demote",
	"reload",
	"shutdown",
	"shortcuts",
	"buyroom"
};
#endif
