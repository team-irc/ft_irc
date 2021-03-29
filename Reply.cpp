#include "Reply.hpp"

Reply::Reply(const int num, std::vector<std::string> param)
{
	if (num == ERR::NOSUCHNICK)
		_msg = this->ERR_NOSUCHNICK(param.at(0));
	if (num == ERR::NOSUCHSERVER)
		_msg = this->ERR_NOSUCHSERVER(param.at(0));
	if (num == ERR::NOSUCHCHANNEL)
		_msg = this->ERR_NOSUCHCHANNEL(param.at(0));
	if (num == ERR::CANNOTSENDTOCHAN)
		_msg = this->ERR_CANNOTSENDTOCHAN(param.at(0));
	if (num == ERR::TOOMANYCHANNELS)
		_msg = this->ERR_TOOMANYCHANNELS(param.at(0));
	if (num == ERR::WASNOSUCHNICK)
		_msg = this->ERR_WASNOSUCHNICK(param.at(0));
	if (num == ERR::TOOMANYTARGETS)
		_msg = this->ERR_TOOMANYTARGET(param.at(0));
	if (num == ERR::NOORIGIN)
		_msg = this->ERR_NOORIGIN();
	if (num == ERR::NORECIPIENT)
		_msg = this->ERR_NORECIPIENT(param.at(0));
	if (num == ERR::NOTEXTTOSEND)
		_msg = this->ERR_NOTEXTTOSEND();
	if (num == ERR::NOTOPLEVEL)
		_msg = this->ERR_NOTOPLEVEL(param.at(0));
	if (num == ERR::WILDTOPLEVEL)
		_msg = this->ERR_WILDTOPLEVEL(param.at(0));
	if (num == ERR::UNKNOWNCOMMAND)
		_msg = this->ERR_UNKNOWNCOMMAND(param.at(0));
	if (num == ERR::NOMOTD)
		_msg = this->ERR_NOMOTD();
	if (num == ERR::NOADMININFO)
		_msg = this->NOADMININFO(param.at(0));
	if (num == ERR::FILEERROR)							
		_msg = this->ERR_FILEERROR(param.at(0), param.at(1));
	if (num == ERR::NONICKNAMEGIVEN)
		_msg = this->ERR_NONICKNAMEGIVEN();
	if (num == ERR::ERRONEUSNICKNAME)
		_msg = this->ERR_ERRONEUSNICKNAME(param.at(0));
	if (num == ERR::NICKNAMEINUSE)
		_msg = this->ERR_NICKNAMEINUSE(param.at(0));
	if (num == ERR::NICKCOLLISION)
		_msg = this->ERR_NICKCOLLISION(param.at(0));
	if (num == ERR::USERNOTINCHANNEL)
		_msg = this->ERR_USERNOTINCHANNEL(param.at(0), param.at(1));
	if (num == ERR::NOTONCHANNEL)
		_msg = this->ERR_NOTONCHANNEL(param.at(0));
	if (num == ERR::USERONCHANNEL)
		_msg = this->ERR_USERONCHANNEL(param.at(0), param.at(1));
	if (num == ERR::NOLOGIN)
		_msg = this->ERR_NOLOGIN(param.at(0));
	if (num == ERR::SUMMONDISABLED)
		_msg = this->ERR_SUMMONDISABLED();
	if (num == ERR::USERSDISABLED)
		_msg = this->ERR_USERSDISABLED();
	if (num == ERR::NOTREGISTERED)
		_msg = this->ERR_NOTREGISTERED();
	if (num == ERR::NEEDMOREPARAMS)						//"<command> :Not enough parameters"
		_msg = param.at(0) + " :Not enough parameters";
	if (num == ERR::ALREADYREGISTRED)					//":You may not reregister"
		_msg = ":You may not reregister";
	if (num == ERR::NOPERMFORHOST)						//":Your host isn't among the privileged"
		_msg = ":Your host isn't among the privileged";
	if (num == ERR::PASSWDMISMATCH)						//":Password incorrect"
		_msg = ":Password incorrect";
	if (num == ERR::YOUREBANNEDCREEP)					//":You are banned from this server"
		_msg = ":You are banned from this server";
	if (num == ERR::KEYSET)								//"<channel> :Channel key already set"
		_msg = param.at(0) + " :Channel key already set";
	if (num == ERR::CHANNELISFULL)						//"<channel> :Cannot join channel (+l)"
		_msg = param.at(0) + " :Cannot join channel (+l)";
	if (num == ERR::UNKNOWNMODE)						//"<char> :is unknown mode char to me"
		_msg = param.at(0) + " :is unknown mode char to me";
	if (num == ERR::INVITEONLYCHAN)						//"<channel> :Cannot join channel (+i)"
		_msg = param.at(0) + " :Cannot join channel (+i)";
	if (num == ERR::BANNEDFROMCHAN)						//"<channel> :Cannot join channel (+b)"
		_msg = param.at(0) + " :Cannot join channel (+b)";
	if (num == ERR::BADCHANNELKEY)						//"<channel> :Cannot join channel (+k)"
		_msg = param.at(0) + " :Cannot join channel (+k)";
	if (num == ERR::NOPRIVILEGES)						//":Permission Denied- You're not an IRC operator"
		_msg = ":Permission Denied- You're not an IRC operator";
	if (num == ERR::CHANOPRIVSNEEDED)					//"<channel> :You're not channel operator"
		_msg = param.at(0) + " :You're not channel operator";
	if (num == ERR::CANTKILLSERVER)						//":You cant kill a server!"
		_msg = ":You cant kill a server!";
	if (num == ERR::NOOPERHOST)							//":No O-lines for your host"
		_msg = ":No O-lines for your host";
	if (num == ERR::UMODEUNKNOWNFLAG)					//":Unknown MODE flag"
		_msg = ":Unknown MODE flag";
	if (num == ERR::USERSDONTMATCH)						//":Cant change mode for other users"
		_msg = ":Cant change mode for other users";
	if (num == RPL::NONE)								//Dummy
		_msg = std::string();
}

Reply::~Reply()
{
}

const std::string & Reply::get_msg()
{
	return (_msg);
}

std::string Reply::ERR_NOSUCHNICK(const std::string & nick)
{
	//"<nickname> :No such nick/channel"
	return (nick + " :No such nick/channel");
}

std::string Reply::ERR_NOSUCHSERVER(const std::string & server_name)
{
	//"<server name> :No such server"
	return (server_name + " :No such server");
}

std::string Reply::ERR_NOSUCHCHANNEL(const std::string & channel_name)
{
	//"<channel name> :No such channel"
	return (channel_name + " :No such channel");
}

std::string Reply::ERR_CANNOTSENDTOCHAN(const std::string & channel_name)
{
	//"<channel name> :Cannot send to channel"
	return (channel_name + " :Cannot send to channel");
}

std::string Reply::ERR_TOOMANYCHANNELS(const std::string & channel_name)
{
	//"<channel name> :You have joined too many channels"
	return (channel_name + " :You have joined too many channels");
}

std::string Reply::ERR_WASNOSUCHNICK(const std::string & nick)
{
	//"<nickname> :There was no such nickname"
	return (nick + " :There was no such nickname");
}

std::string Reply::ERR_TOOMANYTARGETS(const std::string & target)
{
	//"<target> :Duplicate recipients. No message delivered"
	return (target + " :Duplicate recipients. No message delivered");
}

std::string Reply::ERR_NOORIGIN()
{
	//":No origin specified"
	return (":No origin specified");
}

std::string Reply::ERR_NORECIPIENT(const std::string & command)
{
	//":No recipient given (<command>)"
	return (":No recipient given (" + param.at(0) + ")");
}

std::string Reply::ERR_NOTEXTTOSEND()
{
	//":No text to send"
	return (":No text to send");
}

std::string Reply::ERR_NOTOPLEVEL(const std::string & mask)
{
	//"<mask> :No toplevel domain specified"
	return (mask + " :No toplevel domain specified");
}

std::string Reply::ERR_WILDTOPLEVEL(const std::string & mask)
{
	//"<mask> :Wildcard in toplevel domain"
	return (mask + " :Wildcard in toplevel domain");
}

std::string Reply::ERR_UNKNOWNCOMMAND(const std::string & command)
{
	//"<command> :Unknown command"
	return (command + " :Unknown command");
}

std::string Reply::ERR_NOMOTD()
{
	//":MOTD File is missing"
	return (":MOTD File is missing");
}

std::string Reply::ERR_NOADMININFO(const std::string & server)
{
	//"<server> :No administrative info available"
	return (server + " :No administrative info available");
}

std::string Reply::ERR_FILEERROR(const std::string & file_op, const std::string & file)
{
	//":File error doing <file op> on <file>"
	return (":File error doing " + file_op + " on " + file);
}

std::string Reply::ERR_NONICKNAMEGIVEN()
{
	//":No nickname given"
	return (":No nickname given");
}

std::string Reply::ERR_ERRONEUSNICKNAME(const std::string & nick)
{
	//"<nick> :Erroneus nickname"
	return (nick + " :Erroneus nickname");
}

std::string Reply::ERR_NICKNAMEINUSE(const std::string & nick)
{
	//"<nick> :Nickname is already in use"
	return (nick + " :Nickname is already in use");
}

std::string Reply::ERR_NICKCOLLISION(const std::string & nick)
{
	//"<nick> :Nickname collision KILL"
	return (nick + " :Nickname collision KILL");
}

std::string Reply::ERR_USERNOTINCHANNEL(const std::string & nick, const std::string & channel)
{
	//"<nick> <channel> :They aren't on that channel"
	return (nick + ' ' + channel + " :They aren't on that channel");
}

std::string Reply::ERR_NOTONCHANNEL(const std::string & channel)
{
	//"<channel> :You're not on that channel"
	return (channel + " :You're not on that channel");
}

std::string Reply::ERR_USERONCHANNEL(const std::string & user, const std::string & channel)
{
	//"<user> <channel> :is already on channel"
	return (user + ' ' + channel + " :is already on channel");
}

std::string Reply::ERR_NOLOGIN(const std::string & user)
{
	//"<user> :User not logged in"
	return (user + " :User not logged in");
}

std::string Reply::ERR_SUMMONDISABLED()
{
	//":SUMMON has been disabled"
	return (":SUMMON has been disabled");
}

std::string Reply::ERR_USERSDISABLED()
{
	//":USERS has been disabled"
	return (":USERS has been disabled");
}

std::string Reply::ERR_NOTREGISTERED()
{
	//":You have not registered"
	return (":You have not registered");
}