// MUD Programming
// Ron Penton
// (C)2003
// Demo10-01.cpp - SimpleMUD - Enemies, Enemy Databases, and the Game Loop
// 
// 

#include <sstream>
#include <stdlib.h>
#include <time.h>

#include "SocketLib/SocketLib.h"

#include "SimpleMUD/Logon.h"
#include "SimpleMUD/Game.h"
#include "SimpleMUD/GameLoop.h"
#include "SimpleMUD/PlaceNameGenerator.h"

#include "SimpleMUD/SimpleMUDLogs.h"


using namespace SocketLib;
using namespace SimpleMUD;


int main()
{
    try
    {

		// Want to seed the rand for the rest of the program.
		srand(time(NULL));

        GameLoop gameloop;
		PlaceNameGenerator::Load();

        ListeningManager<Telnet, Logon> lm;
        ConnectionManager<Telnet, Logon> cm( 128, 60, 65536 );

        lm.SetConnectionManager( &cm );
		for (int i = 0; i <= 11; i++)
			lm.AddPort( 5100 + i);

        while( Game::Running() )
        {
            lm.Listen();
            cm.Manage();
			gameloop.Loop();
        }
    
    }

    catch( SocketLib::Exception& e )
    {
        ERRORLOG.Log( "Fatal Socket Error: " + e.PrintError() );
    }

    catch( std::exception& e )
    {
        ERRORLOG.Log( "Standard Error: " + std::string( e.what() ) );
    }

    catch( ... )
    {
        ERRORLOG.Log( "Unspecified Error" );
    }



}

