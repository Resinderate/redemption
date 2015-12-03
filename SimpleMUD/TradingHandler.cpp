#include "TradingHandler.h"

void SimpleMUD::TradingHandler::Handle(string p_data)
{
	// Handle the commands for the TradePost.
	string lowerData = BasicLib::LowerCase(p_data);

	if (BasicLib::LowerCase(p_data) == "done")
	{
		m_player->Conn()->RemoveHandler();
		return;
	}

	string first, second, third = "";
	first = BasicLib::ParseWord(p_data, 0);
	second = BasicLib::ParseWord(p_data, 1);
	third = BasicLib::ParseWord(p_data, 2);

	// Can kinda guess we have 3 somewhat valid commands.
	if (first == "" || second == "" || third == "")
	{
		m_player->SendString(SocketLib::red + "Invalid Command! Did not detect 3 values.");
		return;
	}

	resource amount = 0;
	
	try
	{
		amount = std::stoi(second);
	}
	catch (...)
	{
		m_player->SendString(SocketLib::red + "Did not detect valid <amount> type.");
		return;
	}

	// Find the type specified.
	ResourceType type;
	bool valid = false;
	for (int i = 0; i < NumResourceType; i++)
	{
		if (BasicLib::LowerCase(ResourceTypeStrings[i]) == third)
		{
			type = static_cast<ResourceType>(i);
			valid = true;
		}
	}

	if (!valid)
	{
		m_player->SendString(SocketLib::red + "Did not detect valid <resource> type.");
		return;
	}
	// If the type is Wood, won't work.
	if (type == ResourceType::WOOD)
	{
		m_player->SendString(SocketLib::red + "You cannot buy or sell Wood as a resource, as it is the lowest common denominator.");
	}
	ResourceType newType = EvalResouce(type, -1);

	if (first == "buy")
	{
		// Going to upgrade their shit.
		resource newAmount = TradeDown(amount);

		if (m_player->GetResources()[newType] >= newAmount)
		{
			// Take the newType from the player and give them type.
			m_player->GetResources()[newType] -= newAmount;
			m_player->GetResources()[type] += amount;

			// Send string to confirm the trade.
			m_player->SendString(SocketLib::green + "Trade confirmed. " + std::to_string(newAmount) + 
				" " + ResourceTypeStrings[newType] + " -> " + std::to_string(amount) +
				" " + ResourceTypeStrings[type]);
			return;
		}
		else
		{
			// You do not have enough to complete this trade.
			m_player->SendString(SocketLib::red + "You do not have enough resources to complete this trade.");
			return;
		}
	}
	else if (first == "sell")
	{
		// Going to downgrade their shit.
		resource newAmount = TradeDown(amount);

		if (m_player->GetResources()[type] >= amount)
		{
			// Take the type from the player and give them newType.
			m_player->GetResources()[type] -= amount;
			m_player->GetResources()[newType] += newAmount;

			// Send string to confirm trade.
			m_player->SendString(SocketLib::green + "Trade confirmed. " + std::to_string(amount) +
				" " + ResourceTypeStrings[type] + " -> " + std::to_string(newAmount) +
				" " + ResourceTypeStrings[newType]);
			return;
		}
		else
		{
			// Not enough money to complete the trade.
			m_player->SendString(SocketLib::red + "You do not have enough resources to complete this trade.");
			return;
		}

		
	}
	else
	{
		m_player->SendString(SocketLib::red + "Invalid Command!");
		return;
	}
}

void SimpleMUD::TradingHandler::Enter()
{
	// Welcome to the trading centre.
	m_player->SendString(SocketLib::white + "Welcome to the Tradepost!\r\n" +
		"Here you can exchange your resources for others. Each resource can be bought or sold for the resource next in value. Ie. Stone -> Iron\r\n" +
		"Each resource is worth 10 of the previous resource.\r\n"
		"Enter your request in the format:\r\n" +
		SocketLib::yellow + "buy <amount> <resource>" + SocketLib::white + " -- To upgrade a resource. Eg. 'buy 10 iron'. 100 Stone into 10 Iron.\r\n" +
		SocketLib::yellow + "sell <amount> <resource>" + SocketLib::white + " -- To downgrade a resource. Eg 'sell 50 stone'. 50 Stone into 500 Wood.\r\n" +
		"Enter '" + SocketLib::yellow + "done" + SocketLib::white + "' to stop trading.");
}

resource SimpleMUD::TradingHandler::TradeDown(resource p_amount)
{
	return p_amount * 10;
}

SimpleMUD::ResourceType SimpleMUD::TradingHandler::EvalResouce(ResourceType p_origType, int p_indexChange)
{
	return static_cast<ResourceType>(p_origType + p_indexChange);
}
