#include "ServerCommand.hpp"

ServerCommand::ServerCommand() : Command()
{
	// SERVER <servername> <hopcount> <token> <info>
}

ServerCommand::~ServerCommand()
{
}

void	ServerCommand::run(SocketSet	&socket_set)
{
	socket_set.change_socket_type(_msg.get_source_fd(), SERVER);
	std::cout << "SERVER command executed.\n";
}