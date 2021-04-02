#ifndef NAMES_COMMAND_HPP
# define NAMES_COMMAND_HPP

# include "Command.hpp"
# include "Socket.hpp"
# include "Channel.hpp"

class NamesCommand: public Command
{
public:
	NamesCommand();
public:
	void run(IrcServer &irc);
private:
	std::vector<std::string>	get_user_list_who_not_join_any_channel(IrcServer &irc);
};

#endif