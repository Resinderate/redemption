// Class to organise the filtering of bad words from player messages.

// Need a process of storing this out to file properly.

#ifndef LANGUAGEFILTER_H
#define LANGUAGEFILTER_H

#include <fstream>
#include <string>
#include <set>
#include <iterator>
#include <algorithm>
#include <functional>
#include <cctype>

#include "SimpleMUDLogs.h"
#include "../BasicLib/BasicLib.h"

namespace SimpleMUD
{
	// --------------------------------------------------------------------
	//  Class for rooms used for collecting resources.
	// --------------------------------------------------------------------
	class LanguageFilter
	{
	public:
		static bool Filter(std::string& p_message);
		static void Load();

	protected:
		static std::set<std::string> m_blacklist;

	};  // end class LanguageFilter.

}   // end namespace SimpleMUD


#endif
