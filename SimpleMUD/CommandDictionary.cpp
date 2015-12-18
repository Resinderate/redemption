#include "CommandDictionary.h"

SimpleMUD::CommandDictionary::CommandDictionary()
{
}

std::string SimpleMUD::CommandDictionary::Translate(std::string p_command)
{
	std::string initial = p_command;

	// Amount of times you can replace the current word with itself before a timeout.
	int tolerance = 5;
	

	// Only matches the first match.
	// Possible solution would include some forward checking to see if the out command makes any sense.
	for (auto const &kv : m_map)
	{
		USERLOG.Log("Key Val Pair: " + kv.first + "::" + kv.second);
		if (replace(p_command, kv.first, kv.second))
		{

			// If the first word in this data thing, is a valid command, then cool.
			// Otherwise need to kill it I guess.
			std::string firstword = BasicLib::ParseWord(p_command, 0);

			// Check to see if the conversion did anything meaningful.
			if (std::find(BaseCommands.begin(), BaseCommands.end(), firstword) == BaseCommands.end())
			{
				// Not a valid command.
				p_command = initial;
				continue;
			}
			else
			{
				return p_command;
			}
		}
	}
	return p_command;
}

void SimpleMUD::CommandDictionary::AddCommandPair(std::string p_command, std::string p_alias)
{
	// If the key is already present it doesn't add it to the dictionary.
	m_map.insert(std::pair<std::string, std::string>(p_alias, p_command));
}

void SimpleMUD::CommandDictionary::RemoveAlias(std::string p_alias)
{
	m_map.erase(p_alias);
}

bool SimpleMUD::CommandDictionary::replace(std::string & p_str, const std::string & p_from, const std::string & p_to)
{
	size_t start_pos = p_str.find(p_from);
	if (start_pos == std::string::npos)
		return false;
	p_str.replace(start_pos, p_from.length(), p_to);
	return true;
}

int SimpleMUD::CommandDictionary::occurances(const std::string & p_string, const std::string & p_substr)
{
	int count = 0;
	size_t nPos = p_string.find(p_substr, 0);
	while (nPos != std::string::npos)
	{
		count++;
		nPos = p_string.find(p_substr, nPos + 1);
	}

	return count;
}
