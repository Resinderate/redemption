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

sint64 DBSAVETIME = minutes( 1 );
sint64 ANNOUNCE = minutes(1);

void GameLoop::Load()
{
    Game::GetTimer().Reset();
    m_savedatabases = GetTime();
	m_announce = GetTime();
	m_dancing = GetTime();
	m_flipflop = false;

	m_cleanupInactive = GetTime();

    Game::Running() = true;
}


void GameLoop::Save()
{
    std::ofstream file( "game.data" );

    // save the game time
    //file << "[GAMETIME]      "; insert( file, Game::GetTimer().GetMS() ); file << "\n";
    //file << "[SAVEDATABASES] "; insert( file, m_savedatabases );          file << "\n";
}

void GameLoop::Loop()
{
	// Save the databases every 10s.
    if( std::chrono::duration_cast<std::chrono::seconds>(GetTime() - m_savedatabases).count() >= 20)
    {
        SaveDatabases();
		// Reset the last saved time.
		Game::Announce("Saving in Progress");
        m_savedatabases = GetTime();
    }

	if (std::chrono::duration_cast<std::chrono::seconds>(GetTime() - m_announce).count() >= 90)
	{
		++sponsor;
		sponsor = sponsor % 8;
		Game::Sponsor(SPONSORSTRINGS[sponsor]);
		// Reset the last saved time.
		m_announce = GetTime();
	}

	// Could just do it every tick?
	// Kinda wasteful.
	// Go through the currently logged in players and kick anyone that is inactive.
	if (std::chrono::duration_cast<std::chrono::seconds>(GetTime() - m_cleanupInactive).count() >= 5)
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

		m_cleanupInactive = GetTime();
	}

	

	/*
	if (std::chrono::duration_cast<std::chrono::seconds>(GetTime() - m_dancing).count() >= 5)
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
		m_dancing = GetTime();
		
	}
	*/
}


void GameLoop::LoadDatabases()
{
    Load();
    PlayerDatabase::Load();
    //RoomDatabase::LoadTemplates();
    RoomDatabase::LoadData();
	LanguageFilter::Load();
}


void GameLoop::SaveDatabases()
{
    //Save();
    PlayerDatabase::Save();
    RoomDatabase::SaveData();
}
}   // end namespace SimpleMUD

