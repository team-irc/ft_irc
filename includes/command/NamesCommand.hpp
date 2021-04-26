#ifndef NAMES_COMMAND_HPP
# define NAMES_COMMAND_HPP

# include "Command.hpp"
# include "Socket.hpp"
# include "Channel.hpp"

class NamesCommand: public Command
{
public:
	NamesCommand();
	~NamesCommand();
public:
	void run(IrcServer &irc);
private:
	void						reply_all_channel(IrcServer &irc);
	void						reply_specific_channel(IrcServer &irc);
	std::vector<std::string>	get_user_list_who_not_join_any_channel(IrcServer &irc);
};

#endif