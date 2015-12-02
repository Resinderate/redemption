#include "PlayerDictHandler.h"

void SimpleMUD::PlayerDictHandler::Handle(string p_data)
{
	if (BasicLib::LowerCase(p_data) == "done")
	{
		m_connection->RemoveHandler();
		return;
	}
	// Handle Commands.
	if (m_state == RebindState::COMMAND)
	{
		m_command = BasicLib::LowerCase(p_data);
		m_connection->Protocol().SendString(*m_connection, SocketLib::yellow + SocketLib::bold + 
			"Now enter an alias which will map to this command:\r\n" + ">" +
			SocketLib::reset);
		m_state = RebindState::ALIAS;
		return;
	}
	else if (m_state == RebindState::ALIAS)
	{
		m_alias = BasicLib::LowerCase(p_data);
		
		m_player->GetDict().AddCommandPair(m_command, m_alias);
		
		// Confirm alias with player.
		m_connection->Protocol().SendString(*m_connection, SocketLib::yellow + SocketLib::bold +
			"Alias added!\r\n" + ">" +
			SocketLib::reset);
		
		// Return to Game.
		m_connection->RemoveHandler();
		return;
	}
}

void SimpleMUD::PlayerDictHandler::Enter()
{
	m_state = RebindState::COMMAND;

	// Print out instructions.
	m_connection->Protocol().SendString(*m_connection,
		SocketLib::yellow + SocketLib::bold + "First enter a command you want to rebind, then you will be prompted for a shortcut.\r\n" +
		"(Enter 'done' to stop rebinding)\r\n" + ">" +
		SocketLib::reset);
}
