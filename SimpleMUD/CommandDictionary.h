// Class to store a Dictionary of alternate mappings for commands.
// Both the player and the Game with have one of these for alternate commands.
// Ronan

// Need a process of storing this out to file properly.

#ifndef COMMANDDICTIONARY_H
#define COMMANDDICTIONARY_H

#include <string>
#include <map>

#include "SimpleMUDLogs.h"
#include "../BasicLib/BasicLib.h"
#include "Attributes.h"

namespace SimpleMUD
{
	// --------------------------------------------------------------------
	//  Class for rooms used for collecting resources.
	// --------------------------------------------------------------------
	class CommandDictionary
	{
	public:
		CommandDictionary();
		std::string Translate(std::string p_command);
		void AddCommandPair(std::string p_command, std::string p_alias);
		void RemoveAlias(std::string p_alias);
		inline std::map<std::string,std::string> GetDictionary() { return m_map; }
		
	protected:
		std::map<std::string, std::string> m_map;
		bool replace(std::string& p_str, const std::string& p_from, const std::string& p_to);
		int occurances(const std::string& p_string, const std::string& p_substr);

	};  // end class CommandDictionary.

}   // end namespace SimpleMUD


#endif
