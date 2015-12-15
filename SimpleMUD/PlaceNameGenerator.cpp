#include "PlaceNameGenerator.h"

std::vector<std::string> SimpleMUD::PlaceNameGenerator::m_prenames;
std::vector<std::string> SimpleMUD::PlaceNameGenerator::m_postnames;

void SimpleMUD::PlaceNameGenerator::Load()
{
	// Load in the two files to the two vectors of strings.
	std::ifstream prenames("placenames/prenames.txt");
	std::ifstream postnames("placenames/postnames.txt");

	std::string input;

	while (prenames.good())           
	{
		prenames >> input >> std::ws;
		m_prenames.push_back(input);
	}

	while (postnames.good())
	{
		postnames >> input >> std::ws;
		m_postnames.push_back(input);
	}
}

std::string SimpleMUD::PlaceNameGenerator::GetRandomPlaceName()
{
	// Get a random word from each list and return them together to form a place name.
	int first = rand() % m_prenames.size();
	int second = rand() % m_postnames.size();

	return m_prenames[first] + m_postnames[second];
}
