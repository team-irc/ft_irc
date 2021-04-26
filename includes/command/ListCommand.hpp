#ifndef LIST_COMMAND_HPP
# define LIST_COMMAND_HPP

# include "Command.hpp"

class ListCommand: public Command
{
public:
	ListCommand();
	~ListCommand();
public:
	void run(IrcServer &irc);
private:
	void print_list(IrcServer &irc);
	void print_list(IrcServer &irc, std::string *channel_list, int split_size);
};

#endif