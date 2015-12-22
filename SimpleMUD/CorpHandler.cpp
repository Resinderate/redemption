#include "CorpHandler.h"

void SimpleMUD::CorpHandler::Handle(string p_data)
{
	// What happens if someone is already in a guild?
	// Just let them make a new one, and if someone else is in the guild...
	// Pick a random person and make them leader.

	// If someone is just a member of a guild let them make their own guild.

	if (BasicLib::LowerCase(p_data) == "quit")
	{
		m_connection->RemoveHandler();
		return;
	}
	else
	{
		// Check if the data is fine.
			// Some sort of rule names of corps.
			// One word etc.
		
		// Just checking the length for now.
		if (p_data.length() > maxCorpLength)
		{
			m_connection->Protocol().SendString(*m_connection, SocketLib::red + "Corporation name too long!\r\n" + SocketLib::reset + ">");
			return;
		}

		// Change their corp to this.
		m_player->CorpName() = p_data;
		m_player->CorpLeader() = true;

		// Make them the leader.
		m_connection->Protocol().SendString(*m_connection, SocketLib::green + "Corporation created!" + SocketLib::reset);
		m_player->AddTitle(CEO);

		m_connection->Protocol().SendString(*m_connection, SocketLib::green + "Achievement Unlocked! " + GetTitleString(CEO) + SocketLib::reset);

		// Return to game.
		m_connection->RemoveHandler();
		return;
	}
}

void SimpleMUD::CorpHandler::Enter()
{
	// Tell them what they can do.
	m_connection->Protocol().SendString(*m_connection,
		SocketLib::yellow + SocketLib::bold + "Here you are able to create a new corporation that you will be the leader of..\r\n" +
		"Enter a name for your new corporation:\r\n" +
		"A corporation name must be no longer than 20 characters\r\n" +
		"(Enter 'quit' to stop making a corp)\r\n" + ">" +
		SocketLib::reset);
}

