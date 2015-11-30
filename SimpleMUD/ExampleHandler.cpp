#include "ExampleHandler.h"

void SimpleMUD::ExampleHandler::Handle(string p_data)
{
	// Called for each message received.
	if (p_data == "quit")
	{
		m_connection->RemoveHandler();
	}

	m_player->SendString("Got Your Message: " + p_data + ". -- 'quit' to leave handler.");
}

void SimpleMUD::ExampleHandler::Enter()
{
	// Called when you enter the handler for the first time.
}