#ifndef TOPIC_COMMAND_HPP
# define TOPIC_COMMAND_HPP

# include "Command.hpp"

class TopicCommand : public Command
{
private:

public:
	TopicCommand();
	virtual ~TopicCommand();
	TopicCommand(TopicCommand const &copy);
	TopicCommand	&operator=(TopicCommand const &ref);

	void	run(IrcTopic &irc);
};

#endif