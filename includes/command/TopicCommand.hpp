#ifndef TOPIC_COMMAND_HPP
# define TOPIC_COMMAND_HPP

# include "Command.hpp"

/*
Command: TOPIC
Parameter: <channel> [<topic>]

Topic 메세지는 채널의 주제를 변경하거나 보는데 사용됩니다.
주어진 <topic>이 없는 경우 채널에 대한 주제가 리턴됩니다. <topic> 매개 변수가 있는 경우
채널 모드에서 이 작업을 허용하면 해당 채널의 주제가 변경됩니다.
*/

class TopicCommand : public Command
{
private:

public:
	TopicCommand();
	~TopicCommand();
public:
	void	run(IrcServer &irc);
};

#endif