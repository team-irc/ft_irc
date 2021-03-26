#include "Reply.hpp"

Reply::Reply(const int num, std::vector<std::string> param)
{
	if (num == ERR::NOSUCHNICK)							//"<nickname> :No such nick/channel"
		_msg = param.at(0) + " :No such nick/channel";
	if (num == ERR::NOSUCHSERVER)						//"<server name> :No such server"
		_msg = param.at(0) + " :No such server";
	if (num == ERR::NOSUCHCHANNEL)						//"<channel name> :No such channel"
		_msg = param.at(0) + " :No such channel";
	if (num == ERR::CANNOTSENDTOCHAN)					//"<channel name> :Cannot send to channel"
		_msg = param.at(0) + " :Cannot send to channel";
	if (num == ERR::TOOMANYCHANNELS)					//"<channel name> :You have joined too many channels"
		_msg = param.at(0) + " :You have joined too many channels";
	if (num == ERR::WASNOSUCHNICK)						//"<nickname> :There was no such nickname"
		_msg = param.at(0) + " :There was no such nickname";
	if (num == ERR::TOOMANYTARGETS)						//"<target> :Duplicate recipients. No message delivered"
		_msg = param.at(0) + " :Duplicate recipients. No message delivered";
	if (num == ERR::NOORIGIN)							//":No origin specified"
		_msg = ":No origin specified";
	if (num == ERR::NORECIPIENT)						//":No recipient given (<command>)"
		_msg = ":No recipient given (" + param.at(0) + ")";
	if (num == ERR::NOTEXTTOSEND)						//":No text to send"
		_msg = ":No text to send";
	if (num == ERR::NOTOPLEVEL)							//"<mask> :No toplevel domain specified"
		_msg = param.at(0) + " :No toplevel domain specified";
	if (num == ERR::WILDTOPLEVEL)						//"<mask> :Wildcard in toplevel domain"
		_msg = param.at(0) + " :Wildcard in toplevel domain";
	if (num == ERR::UNKNOWNCOMMAND)						//"<command> :Unknown command"
		_msg = param.at(0) + " :Unknown command";
	if (num == ERR::NOMOTD)								//":MOTD File is missing"
		_msg = ":MOTD File is missing";
	if (num == ERR::NOADMININFO)						//"<server> :No administrative info available"
		_msg = param.at(0) + " :No administrative info available";
	if (num == ERR::FILEERROR)							//":File error doing <file op> on <file>"
		_msg = ":File error doing " + paran.at(0) + " on " + param.at(1);
	if (num == ERR::NONICKNAMEGIVEN)					//":No nickname given"
		_msg = ":No nickname given";
	if (num == ERR::ERRONEUSNICKNAME)					//"<nick> :Erroneus nickname"
		_msg = param.at(0) + " :Erroneus nickname";
	if (num == ERR::NICKNAMEINUSE)						//"<nick> :Nickname is already in use"
		_msg = param.at(0) + " :Nickname is already in use";
	if (num == ERR::NICKCOLLISION)						//"<nick> :Nickname collision KILL"
		_msg = param.at(0) + " :Nickname collision KILL";
	if (num == ERR::USERNOTINCHANNEL)					//"<nick> <channel> :They aren't on that channel"
		_msg = param.at(0) + ' ' + param.at(1) + " :They aren't on that channel";
	if (num == ERR::NOTONCHANNEL)						//"<channel> :You're not on that channel"
		_msg = param.at(0) + " :You're not on that channel";
	if (num == ERR::USERONCHANNEL)						//"<user> <channel> :is already on channel"
		_msg = param.at(0) + ' ' + param.at(1) " :is already on channel";
	if (num == ERR::NOLOGIN)							//"<user> :User not logged in"
		_msg = param.at(0) + " :User not logged in";
	if (num == ERR::SUMMONDISABLED)						//":SUMMON has been disabled"
		_msg = ":SUMMON has been disabled";
	if (num == ERR::USERSDISABLED)						//":USERS has been disabled"
		_msg = ":USERS has been disabled";
	if (num == ERR::NOTREGISTERED)						//":You have not registered"
		_msg = ":You have not registered";
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