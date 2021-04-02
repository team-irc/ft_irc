#ifndef LINKS_COMMAND_HPP
# define LINKS_COMMAND_HPP

# include "Command.hpp"

class LinksCommand : public Command
{
private:
	
public:
	LinksCommand();
	virtual ~LinksCommand();
	LinksCommand(LinksCommand const &copy);
	LinksCommand	&operator=(LinksCommand const &ref);

	void	run(IrcServer &irc);
};

#endif