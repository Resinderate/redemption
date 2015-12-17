#include "LeaderboardHandler.h"

void SimpleMUD::LeaderboardHandler::Handle(string p_data)
{
	USERLOG.Log("GOT TO THE LEADERBOARD HANDLE().");

	// Called for each message received.
	if (p_data == "done")
	{
		m_player->SendString("Got Your Message: " + p_data + ". -- 'quit' to leave handler.");
		m_connection->RemoveHandler();
		return;
	}
	else if (p_data == "wood")
	{
		Leaderboard(0);
		return;
	}
	else if (p_data == "stone")
	{

	}
	else if (p_data == "iron")
	{

	}
	else if (p_data == "gold")
	{

	}
	else if (p_data == "corpsoul")
	{

	}
	else
		return;
}

void SimpleMUD::LeaderboardHandler::Leaderboard(int p_data)
{
	std::list<SimpleMUD::Player> pL = SimpleMUD::PlayerDatabase::GetAllPlayers();
	std::map<resource, SimpleMUD::Player> ret;
	int pos = 0;
	switch (p_data)
	{
	case 0:
	{
		for (SimpleMUD::Player p : pL)
		{
			resource res = p.GetResources()[0];
			ret.insert(std::pair<resource, SimpleMUD::Player>(res, p));
		}

		m_player->SendString(SocketLib::white + "Wood Leaderboard:\r\n" +
			"Position\tName\t\tGold\r\n");
		//C++11
		for(auto const &pr : ret)
		{
			Entity temp = pr.second;
			m_player->SendString(SocketLib::white + "" + std::to_string(pos) + "\t" + temp.Name() + "\t" + std::to_string(pr.first));
			++pos;
		}
		break;
	}
	case 1:
		for (SimpleMUD::Player p : pL)
		{
			resource res = p.GetResources()[1];
			ret.insert(std::pair<resource, SimpleMUD::Player>(res, p));
		}
		break;
	case 2:
		for (SimpleMUD::Player p : pL)
		{
			resource res = p.GetResources()[2];
			ret.insert(std::pair<resource, SimpleMUD::Player>(res, p));
		}
		break;
	case 3:
		for (SimpleMUD::Player p : pL)
		{
			resource res = p.GetResources()[3];
			ret.insert(std::pair<resource, SimpleMUD::Player>(res, p));
		}
		break;
	case 4:
		m_player->SendString(SocketLib::white + "Ronans a scrub so this does not work.\r\n");
		break;
	}
}

void SimpleMUD::LeaderboardHandler::Enter()
{
	m_player->SendString(SocketLib::white + "Leaderboards.\r\n" +
		"Enter '" + SocketLib::yellow + "gold" + SocketLib::white + "' for Gold Resource Leaderboard\r\n" +
		"Enter '" + SocketLib::yellow + "iron" + SocketLib::white + "' for Iron Resource Leaderboard\r\n" +
		"Enter '" + SocketLib::yellow + "stone" + SocketLib::white + "' for Stone Resource Leaderboard\r\n" +
		"Enter '" + SocketLib::yellow + "wood" + SocketLib::white + "' for Wood Resource Leaderboard\r\n" +
		"Enter '" + SocketLib::yellow + "corpsoul" + SocketLib::white + "' for Corporation Souls Redeemed Leaderboard\r\n" 
		+ SocketLib::white + ">");
}