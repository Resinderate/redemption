#include "WorkshopHandler.h"

void SimpleMUD::WorkshopHandler::Handle(string p_data)
{
	// Handle the commands for the workshop.
	if (p_data == "done")
	{
		m_player->Conn()->RemoveHandler();
		return;
	}
	if (BasicLib::ParseWord(p_data, 0) == "upgrade")
	{
		string second = "";
		second = BasicLib::ParseWord(p_data, 1);

		if (second == "")
		{
			m_player->SendString(SocketLib::green + "Could not detect valid <resource> type!");
			return;
		}

		
		// Find type
		ResourceType type;
		bool valid = false;
		for (int i = 0; i < NumResourceType; i++)
		{
			if (BasicLib::LowerCase(ResourceTypeStrings[i]) == second)
			{
				type = static_cast<ResourceType>(i);
				valid = true;
			}
		}
		if (!valid)
		{
			m_player->SendString(SocketLib::green + "Did not detect valid <resource> type.");
			return;
		}

		// Check price
		resource price = CalculateCost(m_player->GetItemLevels()[type] + 1);

		// Check if they have enough money
		if (m_player->GetResources()[type] < price)
		{
			m_player->SendString(SocketLib::green + "You don't have enough resources to complete this upgrade!");
			return;
		}

		// Transaction
		m_player->GetResources()[type] -= price;
		m_player->GetItemLevels()[type] += 1;

		// Confirm message.
		m_player->SendString(SocketLib::green + "Upgrade Complete!");
	}
	else
	{
		m_player->SendString(SocketLib::green + "Invalid Command!");
		return;
	}
}

void SimpleMUD::WorkshopHandler::Enter()
{
	// Welcome to the workshop.
	// List of current levels - and cost up upgrade them.
		// Cost could be 1 to get the item. And then a lot higher after that? Start at 1000?
	// Commands that you can use here.

	int woodLvl = m_player->GetItemLevels()[ResourceType::WOOD];
	int stoneLvl = m_player->GetItemLevels()[ResourceType::STONE];
	int ironLvl = m_player->GetItemLevels()[ResourceType::IRON];
	int goldLvl = m_player->GetItemLevels()[ResourceType::GOLD];

	m_player->SendString(SocketLib::green + "Welcome to the Workshop!\r\n" +
		"Here you can upgrade your items for collecting resources.\r\n" +
		"This will increase the number of resources you receive while collecting.\r\n" +
		"Current Item Levels and upgrade costs:\r\n" +
		"Wood lvl: \t" + std::to_string(woodLvl) +
		" -- Upgrade Cost: " + std::to_string(CalculateCost(woodLvl + 1)) + " wood" +
		"\r\n" +
		"Stone lvl:\t" + std::to_string(stoneLvl) +
		" -- Upgrade Cost: " + std::to_string(CalculateCost(stoneLvl +1)) + " stone" +
		"\r\n" +
		"Iron lvl:\t" + std::to_string(ironLvl) +
		" -- Upgrade Cost: " + std::to_string(CalculateCost(ironLvl + 1)) + " iron" +
		"\r\n" +
		"Gold lvl:\t" + std::to_string(goldLvl) +
		" -- Upgrade Cost: " + std::to_string(CalculateCost(goldLvl + 1)) + " gold" +
		"\r\n" +
		"To upgrade use the format: upgrade <type> -- Eg. 'upgrade gold', 'upgrade wood'\r\n" 
		"Enter 'done' to stop upgrading.");

	// Gonna need a function to figure out the price of the upgrade for a particular level.
}

resource SimpleMUD::WorkshopHandler::CalculateCost(int p_level)
{
	resource cost = m_baseCost;
	for (int i = 1; i < p_level; i++)
	{
		cost *= m_multiplier;
	}
	return cost;
}
