// MUD Programming
// Ron Penton
// (C)2003
// GameLoop.cpp - This class performs the game loop in SimpleMUD
// 
// 


#include "../BasicLib/BasicLibTypes.h"
#include "GameLoop.h"
#include <fstream>
#include <string>
using namespace BasicLib;
using namespace SocketLib;

namespace SimpleMUD
{

sint64 DBSAVETIME = minutes( 15 );


void GameLoop::Load()
{
    std::ifstream file( "game.data" );
    file >> std::ws;

    if( file.good() )
    {
        std::string temp;
        sint64 time;
        file >> temp;   extract( file, time );
        Game::GetTimer().Reset();

        file >> temp;   extract( file, m_savedatabases );
    }
    else
    {
        Game::GetTimer().Reset();
        m_savedatabases = DBSAVETIME;
    }

	m_dancing = seconds(1);
	m_flipflop = false;

	m_cleanupInactive = seconds(30);

    Game::Running() = true;
}


void GameLoop::Save()
{
    std::ofstream file( "game.data" );

    // save the game time
    file << "[GAMETIME]      "; insert( file, Game::GetTimer().GetMS() ); file << "\n";
    file << "[SAVEDATABASES] "; insert( file, m_savedatabases );          file << "\n";
}

void GameLoop::Loop()
{
    if( Game::GetTimer().GetMS() >= m_savedatabases )
    {
        SaveDatabases();
        m_savedatabases += DBSAVETIME;
    }

	// Could just do it every tick?
	// Kinda wasteful.
	// Go through the currently logged in players and kick anyone that is inactive.
	if (Game::GetTimer().GetMS() >= m_cleanupInactive)
	{
		for (auto p : PlayerDatabase::GetAllPlayers())
		{
			if (p.LoggedIn())
			{
				int timeout = 120; // seconds, two minutes.
				auto diff = std::chrono::system_clock::now() - p.LatestActivity();
				int secs = std::chrono::duration_cast<std::chrono::seconds>(diff).count();

				if (secs > timeout)
				{
					// Kick the player
					Game::SendGame(red + p.Name() + " has been kicked for inactivity! (2 mins)");
					p.Conn()->Close();
				}
			}
		}

		m_cleanupInactive += seconds(30);
	}

	

	/*
	if (Game::GetTimer().GetMS() > m_dancing)
	{
		string line1, line2, line3, line4, line5, line6, line7, line8 = "";
		// Do alt.
		if (m_flipflop)
		{
			line1 = " _____          _                      _   _             \r\n";
			line2 = "|  __ \\        | |                    | | (_)            \r\n";
			line3 = "| |__) |___  __| | ___ _ __ ___  _ __ | |_ _  ___  _ __  \r\n";
			line4 = "|  _  // _ \\/ _` |/ _ \\ '_ ` _ \\| '_ \\| __| |/ _ \\| '_ \\ \r\n";
			line5 = "| | \\ \\  __/ (_| |  __/ | | | | | |_) | |_| | (_) | | | |\r\n";
			line6 = "|_|  \\_\\___|\\__,_|\\___|_| |_| |_| .__/ \\__|_|\\___/|_| |_|\r\n";
			line7 = "                                | |                      \r\n";
			line8 = "                                |_|     \r\n";
		}
		else
		{
			line1 = "   _____          _                      _   _             \r\n";
			line2 = "  |  __ \\        | |                    | | (_)            \r\n";
			line3 = "  | |__) |___  __| | ___ _ __ ___  _ __ | |_ _  ___  _ __  \r\n";
			line4 = "  |  _  // _ \\/ _` |/ _ \\ '_ ` _ \\| '_ \\| __| |/ _ \\| '_ \\ \r\n";
			line5 = "  | | \\ \\  __/ (_| |  __/ | | | | | |_) | |_| | (_) | | | |\r\n";
			line6 = "  |_|  \\_\\___|\\__,_|\\___|_| |_| |_| .__/ \\__|_|\\___/|_| |_|\r\n";
			line7 = "                                  | |                      \r\n";
			line8 = "                                  |_|     \r\n";
		}
		string out = line1 + line2 + line3 + line4 + line5 + line6 + line7 + line8;
		Game::SendGame(clearscreen + out);

		m_flipflop = !m_flipflop;
		m_dancing += seconds(1);
		
	}
	*/
}


void GameLoop::LoadDatabases()
{
    Load();
    PlayerDatabase::Load();
    //RoomDatabase::LoadTemplates();
    //RoomDatabase::LoadData();
}


void GameLoop::SaveDatabases()
{
    Save();
    PlayerDatabase::Save();
    //RoomDatabase::SaveData();
}
}   // end namespace SimpleMUD

