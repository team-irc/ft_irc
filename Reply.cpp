#include "Reply.hpp"

Reply::Reply(const int num)
{
	if (replies.empty())
	{
		replies.insert(ERR::NOSUCHNICK,			"<nickname> :No such nick/channel");
		replies.insert(ERR::NOSUCHSERVER,		"<server name> :No such server");
		replies.insert(ERR::NOSUCHCHANNEL,		"<channel name> :No such channel");
		replies.insert(ERR::CANNOTSENDTOCHAN,	"<channel name> :Cannot send to channel");
		replies.insert(ERR::TOOMANYCHANNELS,	"<channel name> :You have joined too many channels");
		replies.insert(ERR::WASNOSUCHNICK,		"<nickname> :There was no such nickname");
	}
	_msg = replies.find(num)->second;
}

const std::string & Reply::get_msg()
{
	return (_msg);
}