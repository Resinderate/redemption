#include "ExampleHandler.h"

void SimpleMUD::ExampleHandler::Handle(string p_data)
{
	USERLOG.Log("GOT TO THE SPECIAL ROOM HANDLE().");

	// Called for each message received.
	if (p_data == "quit")
	{
		m_connection->RemoveHandler();
		return;
	}

	m_player->SendString("Got Your Message: " + p_data + ". -- 'quit' to leave handler.");	
}

void SimpleMUD::ExampleHandler::Enter()
{
}