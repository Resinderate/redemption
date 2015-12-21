#include "LanguageFilter.h"

std::set<std::string> SimpleMUD::LanguageFilter::m_blacklist;

bool SimpleMUD::LanguageFilter::Filter(std::string& p_message)
{
	USERLOG.Log("Start Filter: " + p_message);
	bool filtered = false;

	int i = 0;
	std::string word = "";
	std::string censor = "**********************************************";

	while (true)
	{
		// Parse out all the words.
		word = BasicLib::ParseWord(p_message, i);
		if (word == "")
			break;

		// Get rid of the punctuation.
		std::string noPuncWord;

		std::remove_copy_if(word.begin(), word.end(),
			std::back_inserter(noPuncWord),
			std::ptr_fun<int, int>(&std::ispunct)
			);

		
		if (m_blacklist.find(noPuncWord) != m_blacklist.end())
		{
			// Find the position of the original word.
			int position = 0;
			BasicLib::ParseWordInfo(p_message, i, position);
			int len = word.length();

			p_message.replace(position, len, censor.substr(0, len));
			filtered = true;
		}
		
		i++;
	}
	USERLOG.Log("End Filter: " + p_message);
	return filtered;
}

void SimpleMUD::LanguageFilter::Load()
{
	// Load in the list from file.
	std::ifstream file("filters/google_rudewords.txt");
	std::string temp;

	while (file.good())
	{
		file >> temp;
		if (temp == "")
		{
			break;
		}
		m_blacklist.insert(temp);
	}
}
