#include "HandlerFactory.h"

std::shared_ptr<SocketLib::Telnet::handler> SimpleMUD::HandlerFactory::GetHandler(RoomType p_type, SocketLib::Connection<SocketLib::Telnet>& p_conn, player p_player)
{
	// Find the type, and return the appropriate handler.
	if (p_type == RoomType::TRADING)
	{
		return std::shared_ptr<SocketLib::Telnet::handler>(new ExampleHandler(p_conn, p_player));
	}
	/*
	else if (p_type == RoomType::ASSASSIN)
	{
		return 0;
	}
	else if (p_type == RoomType::COPPORATION)
	{
		return 0;
	}
	*/
	else
	{
		return 0;
	}
}
