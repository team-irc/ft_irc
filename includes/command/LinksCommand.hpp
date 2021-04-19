#ifndef LINKS_COMMAND_HPP
# define LINKS_COMMAND_HPP

# include "Command.hpp"

class LinksCommand : public Command
{
private:
	bool	transfer_message(IrcServer &irc, std::string const &server_name, std::string const &mask);
public:
	LinksCommand();
	virtual ~LinksCommand();
	LinksCommand(LinksCommand const &copy);
	LinksCommand	&operator=(LinksCommand const &ref);

	void	run(IrcServer &irc);
};

#endif