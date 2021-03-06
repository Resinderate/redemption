// MUD Programming
// Ron Penton
// (C)2003
// BasicLibTime.h - This file contains all time-based classes and functions


#ifndef BASICLIBTIME_H
#define BASICLIBTIME_H

#include "BasicLibTypes.h"
#include <string>
#include <chrono>

namespace BasicLib
{


    // ============================================================
    // These functions get a time value. The Actual meaning of this 
    // time is undefined; it is only meant to be relative.
    // ============================================================
	std::chrono::system_clock::time_point GetTime();
    
    // ============================================================
    // This prints a timestamp in 24 hours hh:mm:ss format
    // ============================================================
    std::string TimeStamp();


    // ============================================================
    // This prints a datestamp in YYYY:MM:DD format
    // ============================================================
    std::string DateStamp();
    

    // ============================================================
    //  The Timer Class
    // ============================================================
    class Timer
    {
    public:
        
        Timer();
        
        void Reset();
        
        sint64 GetMS();
        sint64 GetS();
        sint64 GetM();
        sint64 GetH();
        sint64 GetD();
        sint64 GetY();

        std::string GetString();



    protected:
		std::chrono::system_clock::time_point m_starttime;

    };


    inline sint64 seconds( sint64 t )   { return t * 1000; }
    inline sint64 minutes( sint64 t )   { return t * 60 * 1000; }
    inline sint64 hours( sint64 t )     { return t * 60 * 60 * 1000; }
    inline sint64 days( sint64 t )      { return t * 24 * 60 * 60 * 1000; }
    inline sint64 weeks( sint64 t )     { return t * 7 * 24 * 60 * 60 * 1000; }
    inline sint64 years( sint64 t )     { return t * 365 * 24 * 60 * 60 * 1000; }


} // end namespace BasicLib


#endif

