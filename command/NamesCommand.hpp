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
	bool						is_he_invisible(Member * member);
	std::vector<std::string>	get_channel_user_list(Channel * channel);
	std::vector<std::string>	get_user_list_who_not_join_any_channel(IrcServer &irc);
};

#endif