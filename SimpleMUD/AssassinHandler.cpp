#include "AssassinHandler.h"

void SimpleMUD::AssassinHandler::Handle(string p_data)
{
	p_data = BasicLib::LowerCase(p_data);
	string firstword = BasicLib::ParseWord(p_data, 0);

	// In the format "murder <player>"
	if (firstword == "murder")
	{
		// Check if they have enough money or whatever? (cost 1 wood).
		if (m_player->GetResources()[WOOD] < 1000)
		{
			m_connection->Protocol().SendString(*m_connection, SocketLib::red + "You don't have enough resources to cover my fee! (1 wood)" + SocketLib::reset);
			return;
		}

		string secondword = BasicLib::ParseWord(p_data, 1);
		if (secondword == "")
		{
			m_connection->Protocol().SendString(*m_connection, SocketLib::red + "Did not detect a players name!" + SocketLib::reset);
			return;
		}

		string secondlower = BasicLib::LowerCase(secondword);

		// Check if the name given is valid.
		auto players = PlayerDatabase::GetAllPlayers();
		for (auto player : players)
		{
			string lowername = BasicLib::LowerCase(player.Name());
			if (lowername == secondlower)
			{
				// Found the correct player. Get the reference rather than the copy.
				Player target = PlayerDatabase::get(player.ID());

				// Kill the target. (take 1 wood off them)
				target.GetResources()[WOOD] -= 1500;
				if (target.GetResources()[WOOD] < 0)
					target.GetResources()[WOOD] = 0;

				m_connection->Protocol().SendString(*m_connection, SocketLib::green + "Killed " + secondword + "!" + 
					SocketLib::reset + ">");
				return;
			}
		}

		m_connection->Protocol().SendString(*m_connection, SocketLib::red + "Could not find a player of that name!" + 
			SocketLib::reset + ">");
		return;
	}
	else if (BasicLib::LowerCase(p_data) == "done")
	{
		// Exit the handler.
		m_connection->RemoveHandler();
	}
	else
	{
		m_player->SendString(SocketLib::red + "Invalid Command!" + 
			SocketLib::reset + ">");
		return;
	}
}

void SimpleMUD::AssassinHandler::Enter()
{
	m_connection->Protocol().SendString(*m_connection, SocketLib::white + "Hello, I am the assassin!\r\n" +
		"Here you can tell me the name of another player you wish me to kill.\r\n" +
		"It will cost 1 wood, and they will lose 1 wood.\r\n" +
		"Enter, 'murder <player name>'.\r\n" +
		"Enter 'done' when you wish to leave.\r\n" +
		SocketLib::reset + ">");
}
