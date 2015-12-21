// MUD Programming
// Ron Penton
// (C)2003
// GameLoop.h - This class performs the game loop in SimpleMUD
// 
// 

#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <string>

#include "PlayerDatabase.h"
#include "RoomDatabase.h"
#include "Game.h"
#include "SimpleMUDLogs.h"

using std::string;

namespace SimpleMUD
{

class GameLoop
{
public:
    GameLoop()      { LoadDatabases(); }
    ~GameLoop()     { SaveDatabases(); }

    void LoadDatabases();       // load all databases
    void SaveDatabases();       // save all databases
    void Loop();                // perform one loop iteration

    void Load();                // load gameloop data
    void Save();                // save gameloop data

protected:
	std::chrono::system_clock::time_point m_savedatabases;
	std::chrono::system_clock::time_point m_announce;
	int sponsor = 0;

	std::chrono::system_clock::time_point m_dancing;
	std::chrono::system_clock::time_point m_cleanupInactive;
	bool m_flipflop;
	

};  // end class GameLoop



}   // end namespace SimpleMUD

#endif
