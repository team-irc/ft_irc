#ifndef COMMAND_SERVER_HPP
# define COMMAND_SERVER_HPP

# include "Command.hpp"

/*
**	서버에 연결 요청하는 커맨드
**	메세지를 받고 다른곳으로 전파
*/
class ServerCommand : public Command
{
private:

public:
	ServerCommand();
	~ServerCommand();
	void	run(SocketSet& socket_set);
};

#endif