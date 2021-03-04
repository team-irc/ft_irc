#ifndef COMMAND_SERVER_HPP
# define COMMAND_SERVER_HPP

# include "../Command.hpp"

// 응답이나, 전파까지
// 메세지
class SERVER : public Command
{
private:

public:
	SERVER();
	~SERVER();
	void	run(const IrcServer& irc);
};

SERVER::SERVER()
{
	// SERVER <servername> <hopcount> <token> <info>
}

SERVER::~SERVER()
{
}

void	SERVER::run(const IrcServer& irc)
{

	// SERVER -> 소켓 타입 바꿈 ->
	irc.get_socket_set().remove_socket();
	tmp->set_type(SockType::SERVER);
	irc.get_socket_set().add_socket();
}

#endif