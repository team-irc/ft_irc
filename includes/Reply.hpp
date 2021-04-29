#ifndef REPLY_HPP
# define REPLY_HPP

# include <iostream>
# include <map>
# include <vector>
# include <string>
# include <set>

class Member;
class Server;
class Channel;

// prefix replies(numeric) nick [error] <msg>
// error는 해당 에러를 발생하는 문자열(ex. No such nick or channel의 경우엔 nick이나 channel의 이름)

namespace ERR
{
	struct		NOSUCHNICK		{ static const int ERRNO = 401; };
	struct		NOSUCHSERVER	{ static const int ERRNO = 402; };
	struct		NOSUCHCHANNEL	{ static const int ERRNO = 403; };
	struct		CANNOTSENDTOCHAN{ static const int ERRNO = 404; };
	struct		TOOMANYCHANNELS	{ static const int ERRNO = 405; };
	struct		WASNOSUCHNICK	{ static const int ERRNO = 406; };
	struct		TOOMANYTARGETS	{ static const int ERRNO = 407; };
	struct		NOORIGIN		{ static const int ERRNO = 409; };
	struct		NORECIPIENT		{ static const int ERRNO = 411; };
	struct		NOTEXTTOSEND	{ static const int ERRNO = 412; };
	struct		NOTOPLEVEL		{ static const int ERRNO = 413; };
	struct		WILDTOPLEVEL	{ static const int ERRNO = 414; };
	struct		UNKNOWNCOMMAND	{ static const int ERRNO = 421; };
	struct		NOMOTD			{ static const int ERRNO = 422; };
	struct		NOADMININFO		{ static const int ERRNO = 423; };
	struct		FILEERROR		{ static const int ERRNO = 424; };
	struct		NONICKNAMEGIVEN	{ static const int ERRNO = 431; };
	struct		ERRONEUSNICKNAME{ static const int ERRNO = 432; };
	struct		NICKNAMEINUSE	{ static const int ERRNO = 433; };
	struct		NICKCOLLISION	{ static const int ERRNO = 436; };
	struct		USERNOTINCHANNEL{ static const int ERRNO = 441; };
	struct		NOTONCHANNEL	{ static const int ERRNO = 442; };
	struct		USERONCHANNEL	{ static const int ERRNO = 443; };
	struct		NOLOGIN			{ static const int ERRNO = 444; };
	struct		SUMMONDISABLED	{ static const int ERRNO = 445; };
	struct		USERSDISABLED	{ static const int ERRNO = 446; };
	struct		NOTREGISTERED	{ static const int ERRNO = 451; };
	struct		NEEDMOREPARAMS	{ static const int ERRNO = 461; };
	struct		ALREADYREGISTRED{ static const int ERRNO = 462; };
	struct		NOPERMFORHOST	{ static const int ERRNO = 463; };
	struct		PASSWDMISMATCH	{ static const int ERRNO = 464; };
	struct		YOUREBANNEDCREEP{ static const int ERRNO = 465; };
	struct		KEYSET			{ static const int ERRNO = 467; };
	struct		CHANNELISFULL	{ static const int ERRNO = 471; };
	struct		UNKNOWNMODE		{ static const int ERRNO = 472; };
	struct		INVITEONLYCHAN	{ static const int ERRNO = 473; };
	struct		BANNEDFROMCHAN	{ static const int ERRNO = 474; };
	struct		BADCHANNELKEY	{ static const int ERRNO = 475; };
	struct		NOPRIVILEGES	{ static const int ERRNO = 481; };
	struct		CHANOPRIVSNEEDED{ static const int ERRNO = 482; };
	struct		CANTKILLSERVER	{ static const int ERRNO = 483; };
	struct		NOOPERHOST		{ static const int ERRNO = 491; };
	struct		UMODEUNKNOWNFLAG{ static const int ERRNO = 501; };
	struct		USERSDONTMATCH	{ static const int ERRNO = 502; };
}

namespace RPL
{
	struct		NONE			{ static const int ERRNO = 300; };
	struct		USERHOST		{ static const int ERRNO = 302; };
	struct		ISON			{ static const int ERRNO = 303; };
	struct		AWAY			{ static const int ERRNO = 301; };
	struct		UNAWAY			{ static const int ERRNO = 305; };
	struct		NOWAWAY			{ static const int ERRNO = 306; };
	struct		WHOISUSER		{ static const int ERRNO = 311; };
	struct		WHOISSERVER		{ static const int ERRNO = 312; };
	struct		WHOISOPERATOR	{ static const int ERRNO = 313; };
	struct		WHOISIDLE		{ static const int ERRNO = 317; };
	struct		ENDOFWHOIS		{ static const int ERRNO = 318; };
	struct		WHOISCHANNELS	{ static const int ERRNO = 319; };
	struct		WHOWASUSER		{ static const int ERRNO = 314; };
	struct		ENDOFWHOWAS		{ static const int ERRNO = 369; };
	struct		LISTSTART		{ static const int ERRNO = 321; };
	struct		LIST			{ static const int ERRNO = 322; };
	struct		LISTEND			{ static const int ERRNO = 323; };
	struct		CHANNELMODEIS	{ static const int ERRNO = 324; };
	struct		NOTOPIC			{ static const int ERRNO = 331; };
	struct		TOPIC			{ static const int ERRNO = 332; };
	struct		INVITING		{ static const int ERRNO = 341; };
	struct		SUMMONING		{ static const int ERRNO = 342; };
	struct		VERSION			{ static const int ERRNO = 351; };
	struct		WHOREPLY		{ static const int ERRNO = 352; };
	struct		ENDOFWHO		{ static const int ERRNO = 315; };
	struct		NAMREPLY		{ static const int ERRNO = 353; };
	struct		ENDOFNAMES		{ static const int ERRNO = 366; };
	struct		LINKS			{ static const int ERRNO = 364; };
	struct		ENDOFLINKS		{ static const int ERRNO = 365; };
	struct		BANLIST			{ static const int ERRNO = 367; };
	struct		ENDOFBANLIST	{ static const int ERRNO = 368; };
	struct		INFO			{ static const int ERRNO = 371; };
	struct		ENDOFINFO		{ static const int ERRNO = 374; };
	struct		MOTDSTART		{ static const int ERRNO = 375; };
	struct		MOTD			{ static const int ERRNO = 372; };
	struct		ENDOFMOTD		{ static const int ERRNO = 376; };
	struct		YOUREOPER		{ static const int ERRNO = 381; };
	struct		REHASHING		{ static const int ERRNO = 382; };
	struct		TIME			{ static const int ERRNO = 391; };
	struct		USERSSTART		{ static const int ERRNO = 392; };
	struct		USERS			{ static const int ERRNO = 393; };
	struct		ENDOFUSERS		{ static const int ERRNO = 394; };
	struct		NOUSERS			{ static const int ERRNO = 395; };
	struct		TRACELINK		{ static const int ERRNO = 200; };
	struct		TRACECONNECTING	{ static const int ERRNO = 201; };
	struct		TRACEHANDSHAKE	{ static const int ERRNO = 202; };
	struct		TRACEUNKNOWN	{ static const int ERRNO = 203; };
	struct		TRACEOPERATOR	{ static const int ERRNO = 204; };
	struct		TRACEUSER		{ static const int ERRNO = 205; };
	struct		TRACESERVER		{ static const int ERRNO = 206; };
	struct		TRACENEWTYPE	{ static const int ERRNO = 208; };
	struct		TRACELOG		{ static const int ERRNO = 261; };
	struct		STATSLINKINFO	{ static const int ERRNO = 211; };
	struct		STATSCOMMANDS	{ static const int ERRNO = 212; };
	struct		STATSCLINE		{ static const int ERRNO = 213; };
	struct		STATSNLINE		{ static const int ERRNO = 214; };
	struct		STATSILINE		{ static const int ERRNO = 215; };
	struct		STATSKLINE		{ static const int ERRNO = 216; };
	struct		STATSYLINE		{ static const int ERRNO = 218; };
	struct		ENDOFSTATS		{ static const int ERRNO = 219; };
	struct		STATSLLINE		{ static const int ERRNO = 241; };
	struct		STATSUPTIME		{ static const int ERRNO = 242; };
	struct		STATSOLINE		{ static const int ERRNO = 243; };
	struct		STATSHLINE		{ static const int ERRNO = 244; };
	struct		UMODEIS			{ static const int ERRNO = 221; };
	struct		LUSERCLIENT		{ static const int ERRNO = 251; };
	struct		LUSEROP			{ static const int ERRNO = 252; };
	struct		LUSERUNKNOWN	{ static const int ERRNO = 253; };
	struct		LUSERCHANNELS	{ static const int ERRNO = 254; };
	struct		LUSERME			{ static const int ERRNO = 255; };
	struct		ADMINME			{ static const int ERRNO = 256; };
	struct		ADMINLOC1		{ static const int ERRNO = 257; };
	struct		ADMINLOC2		{ static const int ERRNO = 258; };
	struct		ADMINEMAIL		{ static const int ERRNO = 259; };
}

class Reply
{
private:
	std::string			_msg;
	static std::string	_server_name;
	static std::string	_user_name;
	std::string			_errnum;
public:
	explicit Reply(ERR::NOSUCHNICK, const std::string &);
	explicit Reply(ERR::NOSUCHSERVER, const std::string &);
	explicit Reply(ERR::NOSUCHCHANNEL, const std::string &);
	explicit Reply(ERR::CANNOTSENDTOCHAN, const std::string &);
	explicit Reply(ERR::TOOMANYCHANNELS, const std::string &);
	explicit Reply(ERR::WASNOSUCHNICK, const std::string &);
	explicit Reply(ERR::TOOMANYTARGETS, const std::string &);
	explicit Reply(ERR::NOORIGIN);
	explicit Reply(ERR::NORECIPIENT, const std::string &);
	explicit Reply(ERR::NOTEXTTOSEND);
	explicit Reply(ERR::NOTOPLEVEL, const std::string &);
	explicit Reply(ERR::WILDTOPLEVEL, const std::string &);
	explicit Reply(ERR::UNKNOWNCOMMAND, const std::string &);
	explicit Reply(ERR::NOMOTD);
	explicit Reply(ERR::NOADMININFO, const std::string &);
	explicit Reply(ERR::FILEERROR, const std::string &, const std::string &);
	explicit Reply(ERR::NONICKNAMEGIVEN);
	explicit Reply(ERR::ERRONEUSNICKNAME, const std::string &);
	explicit Reply(ERR::NICKNAMEINUSE, const std::string &);
	explicit Reply(ERR::NICKCOLLISION, const std::string &);
	explicit Reply(ERR::USERNOTINCHANNEL, const std::string &, const std::string &);
	explicit Reply(ERR::NOTONCHANNEL, const std::string &);
	explicit Reply(ERR::USERONCHANNEL, const std::string &, const std::string &);
	explicit Reply(ERR::NOLOGIN, const std::string &);
	explicit Reply(ERR::SUMMONDISABLED);
	explicit Reply(ERR::USERSDISABLED);
	explicit Reply(ERR::NOTREGISTERED);
	explicit Reply(ERR::NEEDMOREPARAMS, const std::string &);
	explicit Reply(ERR::ALREADYREGISTRED);
	explicit Reply(ERR::NOPERMFORHOST);
	explicit Reply(ERR::PASSWDMISMATCH);
	explicit Reply(ERR::YOUREBANNEDCREEP);
	explicit Reply(ERR::KEYSET, const std::string &);
	explicit Reply(ERR::CHANNELISFULL, const std::string &);
	explicit Reply(ERR::UNKNOWNMODE, const std::string &);
	explicit Reply(ERR::INVITEONLYCHAN, const std::string &);
	explicit Reply(ERR::BANNEDFROMCHAN, const std::string &);
	explicit Reply(ERR::BADCHANNELKEY, const std::string &);
	explicit Reply(ERR::NOPRIVILEGES);
	explicit Reply(ERR::CHANOPRIVSNEEDED, const std::string &);
	explicit Reply(ERR::CANTKILLSERVER);
	explicit Reply(ERR::NOOPERHOST);
	explicit Reply(ERR::UMODEUNKNOWNFLAG);
	explicit Reply(ERR::USERSDONTMATCH);
public:
	explicit Reply(RPL::NONE);
	explicit Reply(RPL::USERHOST, std::vector<Member *>);
	explicit Reply(RPL::YOUREOPER);
	// NAMES
	explicit Reply(RPL::NAMREPLY, const std::string &, std::vector<std::string>);
	explicit Reply(RPL::ENDOFNAMES, const std::string &);
	// LIST
	explicit Reply(RPL::LISTSTART);
	explicit Reply(RPL::LIST, const std::string &, const std::string &, const std::string &);
	explicit Reply(RPL::LISTEND);
	// TOPIC
	explicit Reply(RPL::NOTOPIC, const std::string &);
	explicit Reply(RPL::TOPIC, const std::string &, const std::string &);
	// AWAY
	explicit Reply(RPL::AWAY, const std::string &, const std::string &);
	explicit Reply(RPL::UNAWAY);
	explicit Reply(RPL::NOWAWAY);
	explicit Reply(RPL::INVITING, const std::string &, const std::string &);
	explicit Reply(RPL::VERSION, const std::string &, const std::string &, const std::string &, const std::string &);
	explicit Reply(RPL::ISON, const std::vector<std::string> &);
	explicit Reply(RPL::LINKS, const std::string &, const std::string &, const std::string &, const std::string &);
	explicit Reply(RPL::ENDOFLINKS, const std::string &);
	// INFO
	explicit Reply(RPL::INFO, const std::string &);
	explicit Reply(RPL::ENDOFINFO);
	explicit Reply(RPL::TIME, const std::string &, const std::string &);
	explicit Reply(RPL::REHASHING, const std::string &);
	// WHO
	explicit Reply(RPL::WHOREPLY, Member *, Server *);
	explicit Reply(RPL::ENDOFWHO, const std::string &);
	// WHOIS
	explicit Reply(RPL::WHOISUSER, Member *);
	explicit Reply(RPL::WHOISSERVER, const std::string &, Server *);
	explicit Reply(RPL::WHOISOPERATOR, const std::string &);
	explicit Reply(RPL::WHOISIDLE, const std::string &, int);
	explicit Reply(RPL::ENDOFWHOIS, const std::string &);
	explicit Reply(RPL::WHOISCHANNELS, Member *, std::set<Channel *> &);
	// WHOWAS
	explicit Reply(RPL::WHOWASUSER, Member *);
	explicit Reply(RPL::ENDOFWHOWAS, const std::string &);
	// STATS
	explicit Reply(RPL::STATSUPTIME, const std::string &);
	// ADMIN
	explicit Reply(RPL::ADMINME, const std::string &);
	explicit Reply(RPL::ADMINLOC1, const std::string &);
	explicit Reply(RPL::ADMINLOC2, const std::string &);
	explicit Reply(RPL::ADMINEMAIL, const std::string &);
	//STATS
	explicit Reply(RPL::STATSLINKINFO , const std::string &, const std::string &, const std::string &, const std::string &, 
					const std::string &, const std::string &, const std::string &);
	explicit Reply(RPL::STATSCOMMANDS , const std::string &, const std::string &);
	explicit Reply(RPL::STATSCLINE , const std::string &, const std::string &, const std::string &, const std::string &);
	explicit Reply(RPL::STATSNLINE , const std::string &, const std::string &, const std::string &, const std::string &);
	explicit Reply(RPL::STATSILINE , const std::string &, const std::string &, const std::string &, const std::string &);
	explicit Reply(RPL::STATSKLINE , const std::string &, const std::string &, const std::string &, const std::string &);
	explicit Reply(RPL::STATSYLINE , const std::string &, const std::string &, const std::string &, const std::string &);
	explicit Reply(RPL::ENDOFSTATS , const std::string &);
	explicit Reply(RPL::STATSLLINE , const std::string &, const std::string &, const std::string &);
	explicit Reply(RPL::STATSUPTIME, const time_t birthday);
	explicit Reply(RPL::STATSOLINE , const std::string &, const std::string &);
	explicit Reply(RPL::STATSHLINE , const std::string &, const std::string &);

	//TRACE
	explicit Reply(RPL::TRACELINK, const std::string &, const std::string &, const std::string &);
	explicit Reply(RPL::TRACECONNECTING, const std::string &, const std::string &);
	explicit Reply(RPL::TRACEHANDSHAKE, const std::string &, const std::string &);
	explicit Reply(RPL::TRACEUNKNOWN, const std::string &, const std::string &);

	explicit Reply(RPL::TRACEOPERATOR, const std::string &, const std::string &);
	explicit Reply(RPL::TRACEUSER, const std::string &, const std::string &);
	explicit Reply(RPL::TRACESERVER, const std::string &, int, int, const std::string &, const std::string &);
	explicit Reply(RPL::TRACENEWTYPE, const std::string &, const std::string &);
	explicit Reply(RPL::TRACELOG, const std::string &, const std::string &);
	// MOTD
	explicit Reply(RPL::MOTDSTART, std::string &);
	explicit Reply(RPL::MOTD, std::string &);
	explicit Reply(RPL::ENDOFMOTD);
public:
	~Reply();
	Reply();
public:
	std::string		get_msg() const;
	static void		set_username(std::string const &username);
	static void		set_servername(std::string const &servername);
};

#endif