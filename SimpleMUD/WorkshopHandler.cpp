#include "WorkshopHandler.h"

void SimpleMUD::WorkshopHandler::Handle(string p_data)
{
	// Handle the commands for the workshop.
}

void SimpleMUD::WorkshopHandler::Enter()
{
	// Welcome to the workshop.
	// List of current levels - and cost up upgrade them.
		// Cost could be 1 to get the item. And then a lot higher after that? Start at 1000?
	// Commands that you can use here.

	m_player->SendString(SocketLib::green + "Welcome to the Workshop!\r\n" +
		"Here you can upgrade your items for collecting resources.\r\n" +
		"This will increase the number of resources you receive while collecting.\r\n" +
		"Current Item Levels and upgrade costs:\r\n" +
		"Wood\r\n" +
		"\r\n" +
		"\r\n" +
		"\r\n" +
		"To upgrade use the format: upgrade <type> -- Eg. 'upgrade gold', 'upgrade wood'\r\n" 
		"Enter 'done' to stop upgrading.");

	// Gonna need a function to figure out the price of the upgrade for a particular level.
}
