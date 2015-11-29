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
        Game::GetTimer().Reset( time );

        file >> temp;   extract( file, m_savedatabases );
    }
    else
    {
        Game::GetTimer().Reset();
        m_savedatabases = DBSAVETIME;
    }

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

