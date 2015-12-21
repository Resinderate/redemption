// MUD Programming
// Ron Penton
// (C)2003
// BasicLibTime.cpp - This file contains all time-based classes and functions



#include "BasicLibTime.h"
#include "BasicLibString.h"

// ============================================================
//  Include Files
// ============================================================
#include <time.h>
#ifdef _WIN32
    #include "windows.h"
#else
    #include <sys/time.h>
#endif




namespace BasicLib
{
        // ============================================================
    // These functions get a time value. The Actual meaning of this 
    // time is undefined; it is only meant to be relative.
    // ============================================================
	std::chrono::system_clock::time_point GetTime()
    {
		return std::chrono::system_clock::now();
    }

    // ============================================================
    // This prints a timestamp in 24 hours hh:mm:ss format
    // ============================================================
    std::string TimeStamp()
    {
        char str[9];

        // get the time, and convert it to struct tm format
        time_t a = time(0);
		struct tm b;
        localtime_s(&b, &a);

        // print the time to the string
        strftime( str, 9, "%H:%M:%S", &b );

        return str;
    }


    // ============================================================
    // This prints a datestamp in YYYY:MM:DD format
    // ============================================================
    std::string DateStamp()
    {
        char str[11];

        // get the time, and convert it to struct tm format
        time_t a = time(0);
		struct tm b;
        localtime_s(&b, &a);

        // print the time to the string
        strftime( str, 11, "%Y.%m.%d", &b );

        return str;
    }


    Timer::Timer()
    {
        m_starttime = std::chrono::system_clock::now();
    }


    void Timer::Reset()
    {
        m_starttime = std::chrono::system_clock::now();
    }

    sint64 Timer::GetMS()
    {
        // return the amount of time that has elapsed since the timer
        // was initialized, plus whatever starting time the timer was given.
		return std::chrono::duration_cast<std::chrono::milliseconds>(GetTime() - m_starttime).count();
		
    }

    sint64 Timer::GetS()
    {
        return GetMS() / 1000;
    }

    sint64 Timer::GetM()
    {
        return GetS() / 60;
    }

    sint64 Timer::GetH()
    {
        return GetM() / 60;
    }

    sint64 Timer::GetD()
    {
        return GetH() / 24;
    }
    
    sint64 Timer::GetY()
    {
        return GetD() / 365;
    }

    std::string Timer::GetString()
    {
        std::string str;
        sint64 y = GetY();
        sint64 d = GetD() % 365;
        sint64 h = GetH() % 24;
        sint64 m = GetM() % 60;

        if( y > 0 )
            str += BasicLib::tostring( y ) + " years, ";
        if( d > 0 )
            str += BasicLib::tostring( d ) + " days, ";
        if( h > 0 )
            str += BasicLib::tostring( h ) + " hours, ";
        
        str += BasicLib::tostring( m ) + " minutes";

        return str;
    }


} // end namespace BasicLib
