// MUD Programming
// Ron Penton
// (C)2003
// Demo09-01.cpp - The Maps and Stores and Training Rooms
// 
// 

#include "SocketLib/SocketLib.h"

#include "SimpleMUD/ItemDatabase.h"
#include "SimpleMUD/PlayerDatabase.h"
#include "SimpleMUD/RoomDatabase.h"
#include "SimpleMUD/StoreDatabase.h"

#include "SimpleMUD/Logon.h"
#include "SimpleMUD/Game.h"

#include "SimpleMUD/SimpleMUDLogs.h"


using namespace SocketLib;
using namespace SimpleMUD;

int main()
{

    try
    {
    ItemDatabase::Load();
    PlayerDatabase::Load();
    RoomDatabase::LoadTemplates();
    RoomDatabase::LoadData();
    StoreDatabase::Load();

    ListeningManager<Telnet, Logon> lm;
    ConnectionManager<Telnet, Logon> cm( 128, 60, 65536 );

    lm.SetConnectionManager( &cm );
    lm.AddPort( 5100 );

    Game::GetTimer().Reset();
    Game::Running() = true;

    while( Game::Running() )
    {
        lm.Listen();
        cm.Manage();
        ThreadLib::YieldThread();
    }
    
    }

    catch( SocketLib::Exception& e )
    {
        ERRORLOG.Log( "Fatal Socket Error: " + e.PrintError() );
    }

    catch( ThreadLib::Exception& )
    {
        ERRORLOG.Log( "Fatal Thread Error" );
    }

    catch( std::exception& e )
    {
        ERRORLOG.Log( "Standard Error: " + std::string( e.what() ) );
    }

    catch( ... )
    {
        ERRORLOG.Log( "Unspecified Error" );
    }

    // save the whole database.
    PlayerDatabase::Save();
    RoomDatabase::SaveData();
}

