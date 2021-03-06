#include "LeaderboardHandler.h"

void SimpleMUD::LeaderboardHandler::Handle(string p_data)
{
	// Called for each message received.
	if (p_data == "done")
	{
		m_player->SendString("Got Your Message: " + p_data);
		m_connection->RemoveHandler();
		return;
	}
	else if (p_data == "wood")
	{
		Leaderboard(WOOD);
		return;
	}
	else if (p_data == "stone")
	{
		Leaderboard(STONE);
		return;
	}
	else if (p_data == "iron")
	{
		Leaderboard(IRON);
		return;
	}
	else if (p_data == "gold")
	{
		Leaderboard(GOLD);
		return;
	}
	else if (p_data == "corpsoul")
	{
		Leaderboard(4);
		return;
	}
	else
		return;
}

void SimpleMUD::LeaderboardHandler::Leaderboard(int p_data)
{
	std::list<SimpleMUD::Player> pL = SimpleMUD::PlayerDatabase::GetAllPlayers();
	std::map<resource, SimpleMUD::Player> ret;
	int pos = 1;
	switch (p_data)
	{
	case WOOD:
	{
		for (SimpleMUD::Player p : pL)
		{
			resource res = p.GetResources()[WOOD];
			ret.insert(std::pair<resource, SimpleMUD::Player>(res, p));
		}

		m_player->SendString(SocketLib::white + "Wood Leaderboard:\r\n" +
			"Position Name              Wood\r\n");
		//C++11
		auto &pr = ret.end();
		while (pr != ret.begin())
		{
			pr--;
			Entity temp = pr->second;
			m_player->SendString(SocketLib::white + BufferWord(std::to_string(pos),9) + BufferWord(temp.Name(),18) + std::to_string(pr->first));
			++pos;
		}
		break;
	}
	case STONE:
	{
		for (SimpleMUD::Player p : pL)
		{
			resource res = p.GetResources()[STONE];
			ret.insert(std::pair<resource, SimpleMUD::Player>(res, p));
		}

		m_player->SendString(SocketLib::white + "Stone Leaderboard:\r\n" +
			"Position Name              Stone\r\n");
		//C++11
		auto &pr = ret.end();
		while (pr != ret.begin())
		{
			pr--;
			Entity temp = pr->second;
			m_player->SendString(SocketLib::white + BufferWord(std::to_string(pos), 9) + BufferWord(temp.Name(), 18) + std::to_string(pr->first)); 
			++pos;
		}
		break;
	}
	case IRON:
	{
		for (SimpleMUD::Player p : pL)
		{
			resource res = p.GetResources()[IRON];
			ret.insert(std::pair<resource, SimpleMUD::Player>(res, p));
		}

		m_player->SendString(SocketLib::white + "Iron Leaderboard:\r\n" +
			"Position Name              Iron\r\n");

		//C++11
		auto &pr = ret.end();
		while (pr != ret.begin())
		{
			pr--;
			Entity temp = pr->second;
			m_player->SendString(SocketLib::white + BufferWord(std::to_string(pos), 9) + BufferWord(temp.Name(), 18) + std::to_string(pr->first)); 
			++pos;
		}
		break;
	}
	case GOLD:
	{
		for (SimpleMUD::Player p : pL)
		{
			resource res = p.GetResources()[GOLD];
			ret.insert(std::pair<resource, SimpleMUD::Player>(res, p));
		}

		m_player->SendString(SocketLib::white + "Gold Leaderboard:\r\n" +
			"Position Name              Gold\r\n");
		//C++11
		auto &pr = ret.end();
		while (pr != ret.begin())
		{
			pr--;
			Entity temp = pr->second;
			m_player->SendString(SocketLib::white + BufferWord(std::to_string(pos), 9) + BufferWord(temp.Name(), 18) + std::to_string(pr->first));
			++pos;
		}
		break;
	}
	case 4:
		std::map<int, string> corpRet;
		// Get a list of all the corps.
		auto corpList = PlayerDatabase::AllCorpNames();

		// Get a list of all the members of each corp.
		for (auto corp : corpList)
		{
			// Add up all the totals.
			int totalSouls = 0;
			auto members = PlayerDatabase::CorpMembers(corp);
			for (auto member : members)
			{
				if (member->HasSoul())
					totalSouls += 1;
			}
			// Add to the map.
			corpRet.insert(std::pair<int, string>(totalSouls, corp));
		}

		// Print out some sort of leaderboard.
		// corpRet should have a list of:
		// <int numberOfMembersWithSouls, string corpName>

		
		m_player->SendString(SocketLib::white + "Corporations Leaderboard:\r\n" +
			"Position Corporation              Souls Redeeemed\r\n");
		int pos = 1;
		//C++11
		auto &pr = corpRet.end();
		while (pr != corpRet.begin())
		{
			pr--;
			string temp = pr->second;
		//if (temp != "None")
			//{
				m_player->SendString(SocketLib::white + BufferWord(std::to_string(pos), 9) + BufferWord(temp, 25) + std::to_string(pr->first));
				++pos;
			//}
		}
		break;
	}
}

void SimpleMUD::LeaderboardHandler::Enter()
{
	m_player->SendString(SocketLib::white + "Leaderboards.\r\n" +
		"Enter '" + SocketLib::yellow + "wood" + SocketLib::white + "' for Wood Resource Leaderboard\r\n" +
		"Enter '" + SocketLib::yellow + "stone" + SocketLib::white + "' for Stone Resource Leaderboard\r\n" +
		"Enter '" + SocketLib::yellow + "iron" + SocketLib::white + "' for Iron Resource Leaderboard\r\n" +
		"Enter '" + SocketLib::yellow + "gold" + SocketLib::white + "' for Gold Resource Leaderboard\r\n" +
		"Enter '" + SocketLib::yellow + "corpsoul" + SocketLib::white + "' for Corporation Souls Redeemed Leaderboard\r\n"
		"Enter '" + SocketLib::yellow + "done" + SocketLib::white + "' to return to the game\r\n"
		+ SocketLib::white + ">");
}