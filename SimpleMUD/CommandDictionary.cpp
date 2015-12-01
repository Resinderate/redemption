#include "CommandDictionary.h"

SimpleMUD::CommandDictionary::CommandDictionary()
{
}

std::string SimpleMUD::CommandDictionary::Translate(std::string p_command)
{
	// Only matches the first match.
	// Possible solution would include some forward checking to see if the out command makes any sense.
	for (auto const &kv : m_map)
	{
		if (replace(p_command, kv.first, kv.second))
			return p_command;
	}
	return p_command;
}

void SimpleMUD::CommandDictionary::AddCommandPair(std::string p_command, std::string p_alias)
{
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
