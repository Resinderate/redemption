#include "DevilHandler.h"

void SimpleMUD::DevilHandler::Handle(string p_data)
{
	if (BasicLib::LowerCase(p_data) == "exchange soul")
	{
		if (m_player->GetResources()[GOLD] > m_soulPrice)
		{
			// Take the gold from the player.
			m_player->GetResources()[GOLD] -= m_soulPrice;
			// Give them their soul back.
			m_player->HasSoul() = true;

			m_player->SendString(SocketLib::yellow + "You have been given back your soul!");
			m_connection->RemoveHandler();
		}
		else
		{
			m_player->SendString(SocketLib::yellow + "You do not have enough gold to trade me right now!");
		}
	}
	else if (BasicLib::LowerCase(p_data) == "run away")
	{
		// Exit the handler.
		m_connection->RemoveHandler();
	}
	else
	{
		m_player->SendString(SocketLib::red + "Invalid Command!" + SocketLib::reset);
		return;
	}
}

void SimpleMUD::DevilHandler::Enter()
{
	m_player->SendString(SocketLib::white + "Hello from the devil!\r\n" +
		"You have " + std::to_string(m_player->GetResources()[GOLD]) + " gold. You can trade me back for your soul for " + std::to_string(m_soulPrice) + " gold.\r\n" +
		"Enter '" + SocketLib::yellow + "exchange soul" + SocketLib::white + "' to trade me!\r\n" +
		"Enter '" + SocketLib::yellow + "run away" + SocketLib::white + "' to stop talking to me!\r\n" 
		+ SocketLib::white + ">");
}
