#include "Reply.hpp"

Reply::Reply()
{
}

Reply::Reply(ERR::NOSUCHNICK junk, const std::string & nick)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<nickname> :No such nick/channel"
	_msg = nick + " :No such nick/channel";
}

Reply::Reply(ERR::NOSUCHSERVER junk, const std::string & server_name)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<server name> :No such server"
	_msg = server_name + " :No such server";
}

Reply::Reply(ERR::NOSUCHCHANNEL junk, const std::string & channel_name)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<channel name> :No such channel"
	_msg = channel_name + " :No such channel";
}

Reply::Reply(ERR::CANNOTSENDTOCHAN junk, const std::string & channel_name)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<channel name> :Cannot send to channel"
	_msg = channel_name + " :Cannot send to channel";
}

Reply::Reply(ERR::TOOMANYCHANNELS junk, const std::string & channel_name)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<channel name> :You have joined too many channels"
	_msg = channel_name + " :You have joined too many channels";
}

Reply::Reply(ERR::WASNOSUCHNICK junk, const std::string & nick)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<nickname> :There was no such nickname"
	_msg = nick + " :There was no such nickname";
}

Reply::Reply(ERR::TOOMANYTARGETS junk, const std::string & target)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<target> :Duplicate recipients. No message delivered"
	_msg = target + " :Duplicate recipients. No message delivered";
}

Reply::Reply(ERR::NOORIGIN junk)
{
	_errnum = std::to_string(junk.ERRNO);
	//":No origin specified"
	_msg = ":No origin specified";
}

Reply::Reply(ERR::NORECIPIENT junk, const std::string & command)
{
	_errnum = std::to_string(junk.ERRNO);
	//":No recipient given (<command>)"
	_msg = ":No recipient given (" + command + ")";
}

Reply::Reply(ERR::NOTEXTTOSEND junk)
{
	_errnum = std::to_string(junk.ERRNO);
	//":No text to send"
	_msg = ":No text to send";
}

Reply::Reply(ERR::NOTOPLEVEL junk, const std::string & mask)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<mask> :No toplevel domain specified"
	_msg = mask + " :No toplevel domain specified";
}

Reply::Reply(ERR::WILDTOPLEVEL junk, const std::string & mask)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<mask> :Wildcard in toplevel domain"
	_msg = mask + " :Wildcard in toplevel domain";
}

Reply::Reply(ERR::UNKNOWNCOMMAND junk, const std::string & command)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<command> :Unknown command"
	_msg = command + " :Unknown command";
}

Reply::Reply(ERR::NOMOTD junk)
{
	_errnum = std::to_string(junk.ERRNO);
	//":MOTD File is missing"
	_msg = ":MOTD File is missing";
}

Reply::Reply(ERR::NOADMININFO junk, const std::string & server)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<server> :No administraative info available"
	_msg = server + " :No admianistrative info available";
}

Reply::Reply(ERR::FILEERROR junk, const std::string & file_op, const std::string & file)
{
	_errnum = std::to_string(junk.ERRNO);
	//":File error doing <file aop> on <file>"
	_msg = ":File error doing " + file_op + " on " + file;
}

Reply::Reply(ERR::NONICKNAMEGIVEN junk)
{
	_errnum = std::to_string(junk.ERRNO);
	//":No nickname given"a
	_msg = ":No nickname given";
}

Reply::Reply(ERR::ERRONEUSNICKNAME junk, const std::string & nick)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<nick> :Erroneus nicakname"
	_msg = nick + " :Erroneus nickname";
}

Reply::Reply(ERR::NICKNAMEINUSE junk, const std::string & nick)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<nick> :Nickname is aalready in use"
	_msg = nick + " :Nickname is already in use";
}

Reply::Reply(ERR::NICKCOLLISION junk, const std::string & nick)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<nick> :Nickname collision KILL"
	_msg = nick + " :Nickname collision KILL";
}

Reply::Reply(ERR::USERNOTINCHANNEL junk, const std::string & nick, const std::string & channel)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<nick> <channel> :They aren't on that channel"
	_msg = nick + ' ' + channel + " :They aren't on that channel";
}

Reply::Reply(ERR::NOTONCHANNEL junk, const std::string & channel)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<channel> :You're not on that channel"
	_msg = channel + " :You're not on that channel";
}

Reply::Reply(ERR::USERONCHANNEL junk, const std::string & user, const std::string & channel)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<user> <channel> :is already on channel"
	_msg = user + ' ' + channel + " :is already on channel";
}

Reply::Reply(ERR::NOLOGIN junk, const std::string & user)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<user> :User not logged in"
	_msg = user + " :User not logged in";
}

Reply::Reply(ERR::SUMMONDISABLED junk)
{
	_errnum = std::to_string(junk.ERRNO);
	//":SUMMON has been disabled"
	_msg = ":SUMMON has been disabled";
}

Reply::Reply(ERR::USERSDISABLED junk)
{
	_errnum = std::to_string(junk.ERRNO);
	//":USERS has been disabled"
	_msg = ":USERS has been disabled";
}

Reply::Reply(ERR::NOTREGISTERED junk)
{
	_errnum = std::to_string(junk.ERRNO);
	//":You have not registered"
	_msg = ":You have not registered";
}

Reply::Reply(ERR::NEEDMOREPARAMS junk, const std::string & command)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<command> :Not enough parameters"
	_msg = command + " :Not enough parameters";
}

Reply::Reply(ERR::ALREADYREGISTRED junk)
{
	_errnum = std::to_string(junk.ERRNO);
	//":You may not reregister"
	_msg = ":You may not reregister";
}

Reply::Reply(ERR::NOPERMFORHOST junk)
{
	_errnum = std::to_string(junk.ERRNO);
	//":Your host isn't among the privileged"
	_msg = ":Your host isn't among the privileged";
}

Reply::Reply(ERR::PASSWDMISMATCH junk)
{
	_errnum = std::to_string(junk.ERRNO);
	//":Password incorrect"
	_msg = ":Password incorrect";
}

Reply::Reply(ERR::YOUREBANNEDCREEP junk)
{
	_errnum = std::to_string(junk.ERRNO);
	//":You are banned from this server"
	_msg = ":You are banned from this server";
}

Reply::Reply(ERR::KEYSET junk, const std::string & channel)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<channel> :Channel key already set"
	_msg = channel + " :Channel key already set";
}

Reply::Reply(ERR::CHANNELISFULL junk, const std::string & channel)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<channel> :Cannot join channel (+l)"
	_msg = channel + " :Cannot join channel (+l)";
}

Reply::Reply(ERR::UNKNOWNMODE junk, const std::string & c)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<char> :is unknown mode char to me"
	_msg = c + " :is unknown mode char to me";
}

Reply::Reply(ERR::INVITEONLYCHAN junk, const std::string & channel)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<channel> :Cannot join channel (+i)"
	_msg = channel + " :Cannot join channel (+i)";
}

Reply::Reply(ERR::BANNEDFROMCHAN junk, const std::string & channel)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<channel> :Cannot join channel (+b)"
	_msg = channel + " :Cannot join channel (+b)";
}

Reply::Reply(ERR::BADCHANNELKEY junk, const std::string & channel)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<channel> :Cannot join channel (+k)"
	_msg = channel + " :Cannot join channel (+k)";
}

Reply::Reply(ERR::NOPRIVILEGES junk)
{
	_errnum = std::to_string(junk.ERRNO);
	//":Permission Denied- You're not an IRC operator"
	_msg = ":Permission Denied- You're not an IRC operator";
}

Reply::Reply(ERR::CHANOPRIVSNEEDED junk, const std::string & channel)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<channel> :You're not channel operator"
	_msg = channel + " :You're not channel operator";
}

Reply::Reply(ERR::CANTKILLSERVER junk)
{
	_errnum = std::to_string(junk.ERRNO);
	//":You cant kill a server!"
	_msg = ":You cant kill a server!";
}

Reply::Reply(ERR::NOOPERHOST junk)
{
	_errnum = std::to_string(junk.ERRNO);
	//":No O-lines for your host"
	_msg = ":No O-lines for your host";
}

Reply::Reply(ERR::UMODEUNKNOWNFLAG junk)
{
	_errnum = std::to_string(junk.ERRNO);
	//":Unknown MODE flag"
	_msg = ":Unknown MODE flag";
}

Reply::Reply(ERR::USERSDONTMATCH junk)
{
	_errnum = std::to_string(junk.ERRNO);
	//":Cant change mode for other users"
	_msg = ":Cant change mode for other users";
}

Reply::Reply(RPL::NONE junk)
{
	_errnum = std::to_string(junk.ERRNO);
	// dummy;
	_msg = std::string();
}

Reply::Reply(RPL::YOUREOPER junk)
{
	_errnum = std::to_string(junk.ERRNO);
	//":You are now an IRC operator"
	_msg = ":You are now an IRC operator";
}

Reply::Reply(RPL::NAMREPLY junk, const std::string &channel, std::vector<std::string> names)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
	std::vector<std::string>::iterator first;
	std::vector<std::string>::iterator last;

	first = names.begin();
	last = names.end();
	_msg = channel + " : ";
	while (first != last)
	{
		_msg += *first;
		_msg += ' ';
		++first;
	}
}

Reply::Reply(RPL::ENDOFNAMES junk, const std::string &channel)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<channel> :End of /NAMES list"
	_msg = channel;
	_msg += " :End of /NAMES list";
}

Reply::Reply(RPL::LISTSTART junk)
{
	_errnum = std::to_string(junk.ERRNO);
	//"Channel :Users  Name"
	_msg = "Channel :Users  Name";
}

Reply::Reply(RPL::LIST junk, const std::string &channel, const std::string &visible, const std::string &topic)
{
	_errnum = std::to_string(junk.ERRNO);
	//"<channel> <# visible> :<topic>"
  if (topic[0] == ':')
		_msg = channel + " " + visible + ' ' + topic;
	else
		_msg = channel + " " + visible + " :" + topic;
}

Reply::Reply(RPL::LISTEND junk)
{
	_errnum = std::to_string(junk.ERRNO);
	//":End of /LIST"
	_msg = ":End of /LIST";
}

Reply::~Reply()
{
}

std::string 	Reply::get_msg() const
{
	std::string	ret;

	ret = ":" + _server_name + " " + _errnum + " " + _user_name + " ";
	ret += _msg + "\n";
	return (ret);
}

Reply::Reply(RPL::TOPIC junk, const std::string &channel, const std::string &topic)
{
	_errnum = std::to_string(junk.ERRNO);
	_msg = "#" + channel + " :" + topic;
}

Reply::Reply(RPL::NOTOPIC err, const std::string &channel)
{
	_errnum = std::to_string(err.ERRNO);
	_msg = "#" + channel + " :No topic is set";

void		Reply::set_username(std::string const &username)
{
	_user_name = username;
}
void		Reply::set_servername(std::string const &servername)
{
	_server_name = servername;
}