#include "Reply.hpp"
#include "Member.hpp"
#include "Channel.hpp"
#include "Server.hpp"

Reply::Reply()
{
}

Reply::Reply(ERR::NOSUCHNICK err, const std::string & nick)
{
	_errnum = ft::itos(err.ERRNO);
	//"<nickname> :No such nick/channel"
	_msg = nick + " :No such nick/channel";
}

Reply::Reply(ERR::NOSUCHSERVER err, const std::string & server_name)
{
	_errnum = ft::itos(err.ERRNO);
	//"<server name> :No such server"
	_msg = server_name + " :No such server";
}

Reply::Reply(ERR::NOSUCHCHANNEL err, const std::string & channel_name)
{
	_errnum = ft::itos(err.ERRNO);
	//"<channel name> :No such channel"
	_msg = channel_name + " :No such channel";
}

Reply::Reply(ERR::CANNOTSENDTOCHAN err, const std::string & channel_name)
{
	_errnum = ft::itos(err.ERRNO);
	//"<channel name> :Cannot send to channel"
	_msg = channel_name + " :Cannot send to channel";
}

Reply::Reply(ERR::TOOMANYCHANNELS err, const std::string & channel_name)
{
	_errnum = ft::itos(err.ERRNO);
	//"<channel name> :You have joined too many channels"
	_msg = channel_name + " :You have joined too many channels";
}

Reply::Reply(ERR::WASNOSUCHNICK err, const std::string & nick)
{
	_errnum = ft::itos(err.ERRNO);
	//"<nickname> :There was no such nickname"
	_msg = nick + " :There was no such nickname";
}

Reply::Reply(ERR::TOOMANYTARGETS err, const std::string & target)
{
	_errnum = ft::itos(err.ERRNO);
	//"<target> :Duplicate recipients. No message delivered"
	_msg = target + " :Duplicate recipients. No message delivered";
}

Reply::Reply(ERR::NOORIGIN err)
{
	_errnum = ft::itos(err.ERRNO);
	//":No origin specified"
	_msg = ":No origin specified";
}

Reply::Reply(ERR::NORECIPIENT err, const std::string & command)
{
	_errnum = ft::itos(err.ERRNO);
	//":No recipient given (<command>)"
	_msg = ":No recipient given (" + command + ")";
}

Reply::Reply(ERR::NOTEXTTOSEND err)
{
	_errnum = ft::itos(err.ERRNO);
	//":No text to send"
	_msg = ":No text to send";
}

Reply::Reply(ERR::NOTOPLEVEL err, const std::string & mask)
{
	_errnum = ft::itos(err.ERRNO);
	//"<mask> :No toplevel domain specified"
	_msg = mask + " :No toplevel domain specified";
}

Reply::Reply(ERR::WILDTOPLEVEL err, const std::string & mask)
{
	_errnum = ft::itos(err.ERRNO);
	//"<mask> :Wildcard in toplevel domain"
	_msg = mask + " :Wildcard in toplevel domain";
}

Reply::Reply(ERR::UNKNOWNCOMMAND err, const std::string & command)
{
	_errnum = ft::itos(err.ERRNO);
	//"<command> :Unknown command"
	_msg = command + " :Unknown command";
}

Reply::Reply(ERR::NOMOTD err)
{
	_errnum = ft::itos(err.ERRNO);
	//":MOTD File is missing"
	_msg = ":MOTD File is missing";
}

Reply::Reply(ERR::NOADMININFO err, const std::string & server)
{
	_errnum = ft::itos(err.ERRNO);
	//"<server> :No administraative info available"
	_msg = server + " :No admianistrative info available";
}

Reply::Reply(ERR::FILEERROR err, const std::string & file_op, const std::string & file)
{
	_errnum = ft::itos(err.ERRNO);
	//":File error doing <file aop> on <file>"
	_msg = ":File error doing " + file_op + " on " + file;
}

Reply::Reply(ERR::NONICKNAMEGIVEN err)
{
	_errnum = ft::itos(err.ERRNO);
	//":No nickname given"a
	_msg = ":No nickname given";
}

Reply::Reply(ERR::ERRONEUSNICKNAME err, const std::string & nick)
{
	_errnum = ft::itos(err.ERRNO);
	//"<nick> :Erroneus nicakname"
	_msg = nick + " :Erroneus nickname";
}

Reply::Reply(ERR::NICKNAMEINUSE err, const std::string & nick)
{
	_errnum = ft::itos(err.ERRNO);
	//"<nick> :Nickname is aalready in use"
	_msg = nick + " :Nickname is already in use";
}

Reply::Reply(ERR::NICKCOLLISION err, const std::string & nick)
{
	_errnum = ft::itos(err.ERRNO);
	//"<nick> :Nickname collision KILL"
	_msg = nick + " :Nickname collision KILL";
}

Reply::Reply(ERR::USERNOTINCHANNEL err, const std::string & nick, const std::string & channel)
{
	_errnum = ft::itos(err.ERRNO);
	//"<nick> <channel> :They aren't on that channel"
	_msg = nick + ' ' + channel + " :They aren't on that channel";
}

Reply::Reply(ERR::NOTONCHANNEL err, const std::string & channel)
{
	_errnum = ft::itos(err.ERRNO);
	//"<channel> :You're not on that channel"
	_msg = channel + " :You're not on that channel";
}

Reply::Reply(ERR::USERONCHANNEL err, const std::string & user, const std::string & channel)
{
	_errnum = ft::itos(err.ERRNO);
	//"<user> <channel> :is already on channel"
	_msg = user + ' ' + channel + " :is already on channel";
}

Reply::Reply(ERR::NOLOGIN err, const std::string & user)
{
	_errnum = ft::itos(err.ERRNO);
	//"<user> :User not logged in"
	_msg = user + " :User not logged in";
}

Reply::Reply(ERR::SUMMONDISABLED err)
{
	_errnum = ft::itos(err.ERRNO);
	//":SUMMON has been disabled"
	_msg = ":SUMMON has been disabled";
}

Reply::Reply(ERR::USERSDISABLED err)
{
	_errnum = ft::itos(err.ERRNO);
	//":USERS has been disabled"
	_msg = ":USERS has been disabled";
}

Reply::Reply(ERR::NOTREGISTERED err)
{
	_errnum = ft::itos(err.ERRNO);
	//":You have not registered"
	_msg = ":You have not registered";
}

Reply::Reply(ERR::NEEDMOREPARAMS err, const std::string & command)
{
	_errnum = ft::itos(err.ERRNO);
	//"<command> :Not enough parameters"
	_msg = command + " :Not enough parameters";
}

Reply::Reply(ERR::ALREADYREGISTRED err)
{
	_errnum = ft::itos(err.ERRNO);
	//":You may not reregister"
	_msg = ":You may not reregister";
}

Reply::Reply(ERR::NOPERMFORHOST err)
{
	_errnum = ft::itos(err.ERRNO);
	//":Your host isn't among the privileged"
	_msg = ":Your host isn't among the privileged";
}

Reply::Reply(ERR::PASSWDMISMATCH err)
{
	_errnum = ft::itos(err.ERRNO);
	//":Password incorrect"
	_msg = ":Password incorrect";
}

Reply::Reply(ERR::YOUREBANNEDCREEP err)
{
	_errnum = ft::itos(err.ERRNO);
	//":You are banned from this server"
	_msg = ":You are banned from this server";
}

Reply::Reply(ERR::KEYSET err, const std::string & channel)
{
	_errnum = ft::itos(err.ERRNO);
	//"<channel> :Channel key already set"
	_msg = channel + " :Channel key already set";
}

Reply::Reply(ERR::CHANNELISFULL err, const std::string & channel)
{
	_errnum = ft::itos(err.ERRNO);
	//"<channel> :Cannot join channel (+l)"
	_msg = channel + " :Cannot join channel (+l)";
}

Reply::Reply(ERR::UNKNOWNMODE err, const std::string & c)
{
	_errnum = ft::itos(err.ERRNO);
	//"<char> :is unknown mode char to me"
	_msg = c + " :is unknown mode char to me";
}

Reply::Reply(ERR::INVITEONLYCHAN err, const std::string & channel)
{
	_errnum = ft::itos(err.ERRNO);
	//"<channel> :Cannot join channel (+i)"
	_msg = channel + " :Cannot join channel (+i)";
}

Reply::Reply(ERR::BANNEDFROMCHAN err, const std::string & channel)
{
	_errnum = ft::itos(err.ERRNO);
	//"<channel> :Cannot join channel (+b)"
	_msg = channel + " :Cannot join channel (+b)";
}

Reply::Reply(ERR::BADCHANNELKEY err, const std::string & channel)
{
	_errnum = ft::itos(err.ERRNO);
	//"<channel> :Cannot join channel (+k)"
	_msg = channel + " :Cannot join channel (+k)";
}

Reply::Reply(ERR::NOPRIVILEGES err)
{
	_errnum = ft::itos(err.ERRNO);
	//":Permission Denied- You're not an IRC operator"
	_msg = ":Permission Denied- You're not an IRC operator";
}

Reply::Reply(ERR::CHANOPRIVSNEEDED err, const std::string & channel)
{
	_errnum = ft::itos(err.ERRNO);
	//"<channel> :You're not channel operator"
	_msg = channel + " :You're not channel operator";
}

Reply::Reply(ERR::CANTKILLSERVER err)
{
	_errnum = ft::itos(err.ERRNO);
	//":You cant kill a server!"
	_msg = ":You cant kill a server!";
}

Reply::Reply(ERR::NOOPERHOST err)
{
	_errnum = ft::itos(err.ERRNO);
	//":No O-lines for your host"
	_msg = ":No O-lines for your host";
}

Reply::Reply(ERR::UMODEUNKNOWNFLAG err)
{
	_errnum = ft::itos(err.ERRNO);
	//":Unknown MODE flag"
	_msg = ":Unknown MODE flag";
}

Reply::Reply(ERR::USERSDONTMATCH err)
{
	_errnum = ft::itos(err.ERRNO);
	//":Cant change mode for other users"
	_msg = ":Cant change mode for other users";
}

Reply::Reply(RPL::NONE err)
{
	_errnum = ft::itos(err.ERRNO);
	// dummy;
	_msg = std::string();
}

Reply::Reply(RPL::USERHOST reply, std::vector<Member *> member_list)
{
	_errnum = ft::itos(reply.ERRNO);
	// ":[<reply>{<space><reply>}]"
	std::vector<Member *>::iterator first = member_list.begin();
	std::vector<Member *>::iterator last = member_list.end();

	_msg += ":";
	while (first != last)
	{
		_msg += (*first)->get_nick();
		if ((*first)->check_mode('o', false))
			_msg += '*';
		_msg += " = ";
		if ((*first)->get_away().empty())
			_msg += '+';
		else
			_msg += '-';
		_msg += (*first)->get_hostname();
		if (first + 1 != last)
			_msg += ' ';
		++first;
	}
}

Reply::Reply(RPL::YOUREOPER rpl)
{
	_errnum = ft::itos(rpl.ERRNO);
	//":You are now an IRC operator"
	_msg = ":You are now an IRC operator";
}

Reply::Reply(RPL::NAMREPLY rpl, const std::string &channel, std::vector<std::string> names)
{
	_errnum = ft::itos(rpl.ERRNO);
	//"<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
	std::vector<std::string>::iterator first;
	std::vector<std::string>::iterator last;

	first = names.begin();
	last = names.end();
	_msg = channel + " :";
	while (first != last)
	{
		_msg += *first;
		_msg += ' ';
		++first;
	}
}

Reply::Reply(RPL::ENDOFNAMES rpl, const std::string &channel)
{
	_errnum = ft::itos(rpl.ERRNO);
	//"<channel> :End of /NAMES list"
	_msg = channel;
	_msg += " :End of NAMES list";
}

Reply::Reply(RPL::LISTSTART rpl)
{
	_errnum = ft::itos(rpl.ERRNO);
	//"Channel :Users  Name"
	_msg = "Channel :Users  Name";
}

Reply::Reply(RPL::LIST rpl, const std::string &channel, const std::string &visible, const std::string &topic)
{
	_errnum = ft::itos(rpl.ERRNO);
	//"<channel> <# visible> :<topic>"
	_msg = channel + " " + visible + " " + topic;
}

Reply::Reply(RPL::LISTEND rpl)
{
	_errnum = ft::itos(rpl.ERRNO);
	//":End of /LIST"
	_msg = ":End of /LIST";
}

Reply::Reply(RPL::TOPIC rpl, const std::string &channel, const std::string &topic)
{
	_errnum = ft::itos(rpl.ERRNO);
	//"<channel> :<topic>"
	_msg = channel + " :" + topic;
}

Reply::Reply(RPL::AWAY rpl, const std::string &nick, const std::string &away_message)
{
	_errnum = ft::itos(rpl.ERRNO);
	//"<nick> :<away message>"
	_msg = nick + " " + away_message;
}

Reply::Reply(RPL::UNAWAY rpl)
{
	_errnum = ft::itos(rpl.ERRNO);
	//":You are no longer marked as being away"
	_msg = ":You are no longer marked as being away";
}

Reply::Reply(RPL::NOWAWAY rpl)
{
	_errnum = ft::itos(rpl.ERRNO);
	//":You have been marked as being away"
	_msg = ":You have been marked as being away";
}

Reply::Reply(RPL::INVITING rpl, const std::string &channel_name, const std::string &nick_name)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = channel_name + " " + nick_name;
}

Reply::Reply(RPL::ISON rpl, const std::vector<std::string> & name_list)
{
	_errnum = ft::itos(rpl.ERRNO);
	std::vector<std::string>::const_iterator first = name_list.begin();
	std::vector<std::string>::const_iterator last = name_list.end();

	_msg += ": ";
	while (first != last)
	{
		_msg += *first;
		if (first + 1 != last)
			_msg += " ";
		++first;
	}
}

Reply::Reply(RPL::INFO rpl, const std::string & string)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg += ":" + string;
}

Reply::Reply(RPL::ENDOFINFO rpl)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = ":End of /INFO list";
}

Reply::Reply(RPL::NOTOPIC err, const std::string &channel)
{
	_errnum = ft::itos(err.ERRNO);
	_msg = "#" + channel + " :No topic is set";
}

Reply::Reply(RPL::VERSION rpl, const std::string &version, const std::string &debug_level, 
				const std::string &server, const std::string &comments)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = version + "." + debug_level + " " + server + " :" + comments;
}

Reply::Reply(RPL::LINKS rpl, const std::string &mask, const std::string &server, const std::string &hopcount, const std::string &server_info)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = mask + " " + server + " :" + hopcount + " " + server_info;
}


Reply::Reply(RPL::ENDOFLINKS rpl, const std::string &mask)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = mask + " :End of /LINKS list";
}

Reply::Reply(RPL::TIME time, const std::string &servername, const std::string &val)
{
	_errnum = ft::itos(time.ERRNO);
	_msg = servername + " :" + val;
}

Reply::Reply(RPL::REHASHING rpl, const std::string &config_file)
{
	_errnum = ft::itos(rpl.ERRNO);
	// "<config file> :Rehashing"
	_msg = config_file + " :Rehashing";
}

Reply::Reply(RPL::WHOREPLY rpl, Member * member, Server * server)
{
	_errnum = ft::itos(rpl.ERRNO);
	//"<channel> <user> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <real name>"
	_msg = member->get_username() + ' ' + member->get_hostname() + ' ' + member->get_servername() + ' ' + \
		member->get_nick() + " :" + ft::itos(server->get_hopcount()) + ' ' + member->get_realname();
}

Reply::Reply(RPL::ENDOFWHO rpl, const std::string & name)
{
	_errnum = ft::itos(rpl.ERRNO);
	// "<name> :End of /WHO list"
	_msg = name + " :End of /WHO list";
}

// WHOIS

Reply::Reply(RPL::WHOISUSER rpl, Member * member)
{
	_errnum = ft::itos(rpl.ERRNO);
	// "<nick> <user> <host> * :<real name>"
	_msg = member->get_nick() + ' ' + member->get_username() + ' ' + member->get_hostname() + " * :" + member->get_realname();
}

Reply::Reply(RPL::WHOISSERVER rpl, const std::string & nick, Server * server)
{
	_errnum = ft::itos(rpl.ERRNO);
	// "<nick> <server> :<server info>"
	_msg = nick + ' ' + server->get_name() + " " + server->get_info();
}

Reply::Reply(RPL::WHOISOPERATOR rpl, const std::string & nick)
{
	_errnum = ft::itos(rpl.ERRNO);
	// "<nick> :is an IRC operator"
	_msg = nick + " :is an IRC operator";
}

Reply::Reply(RPL::WHOISIDLE rpl, const std::string & nick, int integer)
{
	_errnum = ft::itos(rpl.ERRNO);
	// "<nick> <integer> :seconds idle"
	_msg = nick + ' ' + ft::itos(integer) + " :seconds idle";
}

Reply::Reply(RPL::ENDOFWHOIS rpl, const std::string & nick)
{
	_errnum = ft::itos(rpl.ERRNO);
	// "<nick> :End of /WHOIS list"
	_msg = nick + " :End of /WHOIS list";
}

Reply::Reply(RPL::WHOISCHANNELS rpl, Member * member, std::set<Channel *> & joined_channels)
{
	_errnum = ft::itos(rpl.ERRNO);
	// "<nick> :{[@|+]<channel><space>}"
	std::set<Channel *>::iterator first = joined_channels.begin();
	std::set<Channel *>::iterator last = joined_channels.end();
	_msg = member->get_nick() + " :";
	while (first != last)
	{
		char permission;
		Channel * channel = *first;
		if (channel->is_operator(member))
			permission = '@';
		else if (channel->is_voice(member))
			permission = '+';
		_msg += permission + channel->get_name() + ' ';
		++first;
	}
}

// WHOWAS

Reply::Reply(RPL::WHOWASUSER rpl, Member * member)
{
	_errnum = ft::itos(rpl.ERRNO);
	// "<nick> <user> <host> * :<real name>"
	_msg = member->get_nick() + ' ' + member->get_username() + ' ' + member->get_hostname() + " * :" + member->get_realname();
}

Reply::Reply(RPL::ENDOFWHOWAS rpl, const std::string & nick)
{
	_errnum = ft::itos(rpl.ERRNO);
	// "<nick> :End of WHOWAS"
	_msg = nick + " :End of WHOWAS";
}

// STATS

Reply::Reply(RPL::STATSUPTIME time, const std::string &val)
{
	_errnum = ft::itos(time.ERRNO);
	_msg = ":" + val;
}

// ADMIN

Reply::Reply(RPL::ADMINME admin, const std::string &server)
{
	_errnum = ft::itos(admin.ERRNO);
	_msg = server + " :Administrative info";
}

Reply::Reply(RPL::ADMINLOC1 adloc1, const std::string &info)
{
	_errnum = ft::itos(adloc1.ERRNO);
	_msg = ":" + info;
}

Reply::Reply(RPL::ADMINLOC2 adloc2, const std::string &info)
{
	_errnum = ft::itos(adloc2.ERRNO);
	_msg = ":" + info;
}

Reply::Reply(RPL::ADMINEMAIL admail, const std::string &info)
{
	_errnum = ft::itos(admail.ERRNO);
	_msg = ":" + info;
}

// TRACE

Reply::Reply(RPL::TRACELINK rpl, const std::string &version, const std::string &dest, const std::string &next_server)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = "Link " + version + " " + dest + " " + next_server;
}

Reply::Reply(RPL::TRACECONNECTING rpl, const std::string &class_str, const std::string &server)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = "Try. " + class_str + " " + server;
}

Reply::Reply(RPL::TRACEHANDSHAKE rpl, const std::string &class_str, const std::string &server)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = "H.S. " + class_str + " " + server;
}

Reply::Reply(RPL::TRACEUNKNOWN rpl, const std::string &class_str, const std::string &client_ip)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = "??? " + class_str + " " + client_ip;
}

Reply::Reply(RPL::TRACEOPERATOR rpl, const std::string &class_str, const std::string &nickname)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = "Oper " + class_str + " " + nickname;
}

Reply::Reply(RPL::TRACEUSER rpl, const std::string &class_str, const std::string &nickname)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = "User " + class_str + " " + nickname;
}

Reply::Reply(RPL::TRACESERVER rpl, const std::string &class_str, int s, int c, 
			const std::string &server, const std::string &linkname)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = "Serv " + class_str + " " + ft::itos(s) + "S " + ft::itos(c) + "C " + server;
	_msg += " ";
	_msg += linkname;
}

Reply::Reply(RPL::TRACENEWTYPE rpl, const std::string &newtype, const std::string &client_name)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = newtype + " 0 " + client_name;
}

Reply::Reply(RPL::TRACELOG rpl, const std::string &logfile, const std::string &debug_level)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = "File " + logfile + " " + debug_level;
}

// STATS
Reply::Reply(RPL::STATSLINKINFO rpl, const std::string &linkname, const std::string &sendq, const std::string &sent_msg, 
	const std::string &sent_bytes, const std::string &recv_msg, const std::string &recv_bytes, const std::string &time_open)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = linkname + " " + sendq + " " + sent_msg + " " + sent_bytes + " " + recv_msg + " " + recv_bytes + " " + time_open;
}

Reply::Reply(RPL::STATSCOMMANDS rpl, const std::string &command, const std::string &count)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = command + " " + count;
}

Reply::Reply(RPL::STATSCLINE rpl, const std::string &host, const std::string &name, const std::string &port, const std::string &class_name)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = "C " + host + " * " + name + " " + port + " " + class_name;
}

Reply::Reply(RPL::STATSNLINE rpl, const std::string &host, const std::string &name, const std::string &port, const std::string &class_name)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = "N " + host + " * " + name + " " + port + " " + class_name;
}

Reply::Reply(RPL::STATSILINE rpl, const std::string &host, const std::string &host2, const std::string &port, const std::string &class_name)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = "I " + host + " * " + host2 + " " + port + " " + class_name;
}

Reply::Reply(RPL::STATSKLINE rpl, const std::string &host, const std::string &username, const std::string &port, const std::string &class_name)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = "K " + host + " * " + username + " " + port + " " + class_name;
}

Reply::Reply(RPL::STATSYLINE rpl, const std::string &class_name, const std::string &ping_freq, const std::string &conn_freq, const std::string &max_sendq)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = "Y " + class_name + " " + ping_freq + " " + conn_freq + " " + max_sendq;
}

Reply::Reply(RPL::ENDOFSTATS rpl, const std::string &flag)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = flag + " :End of STATS report";
}

Reply::Reply(RPL::STATSLLINE rpl, const std::string &hostmask, const std::string &servername, const std::string &max_depth)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = "L " + hostmask + " " + servername + " " + max_depth;
}

Reply::Reply(RPL::STATSUPTIME rpl, const time_t birthday)
{
	time_t			div;
	time_t			rem;

	_errnum = ft::itos(rpl.ERRNO);
	time(&div);
	div = div - birthday;
	rem = div % 86400;
	div = div / 86400;
	
	_msg = ":Server Up " + ft::itos(div) + " days ";

	div = rem / 3600;
	rem = rem % 3600;

	_msg += ft::itos(div) + ":";

	div = rem / 60;
	rem = rem % 60;

	_msg += ft::itos(div) + ":" + ft::itos(rem);
}

Reply::Reply(RPL::STATSOLINE rpl, const std::string &hostmask, const std::string &name)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = "O " + hostmask + " * " + name;
}

Reply::Reply(RPL::STATSHLINE rpl, const std::string &hostmask, const std::string &server_name)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = "H " + hostmask + " * " + server_name;
}

// MOTD

Reply::Reply(RPL::MOTDSTART rpl, std::string & server)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = ":- " + server + " Message of the day - ";
}

Reply::Reply(RPL::MOTD rpl, std::string & text)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = ":- " + text;
}

Reply::Reply(RPL::ENDOFMOTD rpl)
{
	_errnum = ft::itos(rpl.ERRNO);
	_msg = ":End of /MOTD command";
}

void		Reply::set_username(std::string const &username)
{
	_user_name = username;
}

void		Reply::set_servername(std::string const &servername)
{
	_server_name = servername;
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