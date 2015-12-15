// Class to generate random place names.
// Ronan

#ifndef PLACENAMEGEN_H
#define PLACENAMEGEN_H

#include <string>
#include <vector>
#include <stdlib.h>

#include "SimpleMUDLogs.h"

namespace SimpleMUD
{
	// --------------------------------------------------------------------
	//  Class for rooms used for collecting resources.
	// --------------------------------------------------------------------
	class PlaceNameGenerator
	{
	public:
		static void Load();
		static std::string GetRandomPlaceName();

	protected:
		static std::vector<std::string> m_prenames;
		static std::vector<std::string> m_postnames;

	};  // end class PlaceNameGenerator.

}   // end namespace SimpleMUD


#endif
