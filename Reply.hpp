#ifndef REPLY_HPP
# define REPLY_HPP

# include <iostream>
# include <map>

// prefix replies(numeric) nick [error] <msg>
// error는 해당 에러를 발생하는 문자열(ex. No such nick or channel의 경우엔 nick이나 channel의 이름)

namespace ERR
{
	const int		NOSUCHNICK			= 401;
	const int		NOSUCHSERVER		= 402;
	const int		NOSUCHCHANNEL		= 403;
	const int		CANNOTSENDTOCHAN	= 404;
	const int		TOOMANYCHANNELS		= 405;
	const int		WASNOSUCHNICK		= 406;
	const int		TOOMANYTARGETS		= 407;
	const int		NOORIGIN			= 409;
	const int		NORECIPIENT			= 411;
	const int		NOTEXTTOSEND		= 412;
	const int		NOTOPLEVEL			= 413;
	const int		WILDTOPLEVEL		= 414;
	const int		UNKNOWNCOMMAND		= 421;
	const int		NOMOTD				= 422;
	const int		NOADMININFO			= 423;
	const int		FILEERROR			= 424;
	const int		NONICKNAMEGIVEN		= 431;
	const int		ERRONEUSNICKNAME	= 432;
	const int		NICKNAMEINUSE		= 433;
	const int		NICKCOLLISION		= 436;
	const int		USERNOTINCHANNEL	= 441;
	const int		NOTONCHANNEL		= 442;
	const int		USERONCHANNEL		= 443;
	const int		NOLOGIN				= 444;
	const int		SUMMONDISABLED		= 445;
	const int		USERSDISABLED		= 446;
	const int		NOTREGISTERED		= 451;
	const int		NEEDMOREPARAMS		= 461;
	const int		ALREADYREGISTRED	= 462;
	const int		NOPERMFORHOST		= 463;
	const int		PASSWDMISMATCH		= 464;
	const int		YOUREBANNEDCREEP	= 465;
	const int		KEYSET				= 467;
	const int		CHANNELISFULL		= 471;
	const int		UNKNOWNMODE			= 472;
	const int		INVITEONLYCHAN		= 473;
	const int		BANNEDFROMCHAN		= 474;
	const int		BADCHANNELKEY		= 475;
	const int		NOPRIVILEGES		= 481;
	const int		CHANOPRIVSNEEDED	= 482;
	const int		CANTKILLSERVER		= 483;
	const int		NOOPERHOST			= 491;
	const int		UMODEUNKNOWNFLAG	= 501;
	const int		USERSDONTMATCH		= 502;
}

namespace RPL
{
	const int		NONE				= 300;
	const int		USERHOST			= 302;
	const int		ISON				= 303;
	const int		AWAY				= 301;
	const int		UNAWAY				= 305;
	const int		NOWAWAY				= 306;
	const int		WHOISUSER			= 311;
	const int		WHOISSERVER			= 312;
	const int		WHOISOPERATOR		= 313;
	const int		WHOISIDLE			= 317;
	const int		ENDOFWHOIS			= 318;
	const int		WHOISCHANNELS		= 319;
	const int		WHOWASUSER			= 314;
	const int		ENDOFWHOWAS			= 369;
	const int		LISTSTART			= 321;
	const int		LIST				= 322;
	const int		LISTEND				= 323;
	const int		CHANNELMODEIS		= 324;
	const int		NOTOPIC				= 331;
	const int		TOPIC				= 332;
	const int		INVITING			= 341;
	const int		SUMMONING			= 342;
	const int		VERSION				= 351;
	const int		WHOREPLY			= 352;
	const int		ENDOFWHO			= 315;
	const int		NAMREPLY			= 353;
	const int		ENDOFNAMES			= 366;
	const int		LINKS				= 364;
	const int		ENDOFLINKS			= 365;
	const int		BANLIST				= 367;
	const int		ENDOFBANLIST		= 368;
	const int		INFO				= 371;
	const int		ENDOFINFO			= 374;
	const int		MOTDSTART			= 375;
	const int		MOTD				= 372;
	const int		ENDOFMOTD			= 376;
	const int		YOUREOPER			= 381;
	const int		REHASHING			= 382;
	const int		TIME				= 391;
	const int		USERSSTART			= 392;
	const int		USERS				= 393;
	const int		ENDOFUSERS			= 394;
	const int		NOUSERS				= 395;
	const int		TRACELINK			= 200;
	const int		TRACECONNECTING		= 201;
	const int		TRACEHANDSHAKE		= 202;
	const int		TRACEUNKNOWN		= 203;
	const int		TRACEOPERATOR		= 204;
	const int		TRACEUSER			= 205;
	const int		TRACESERVER			= 206;
	const int		TRACENEWTYPE		= 208;
	const int		TRACELOG			= 261;
	const int		STATSLINKINFO		= 211;
	const int		STATSCOMMANDS		= 212;
	const int		STATSCLINE			= 213;
	const int		STATSNLINE			= 214;
	const int		STATSILINE			= 215;
	const int		STATSKLINE			= 216;
	const int		STATSYLINE			= 218;
	const int		ENDOFSTATS			= 219;
	const int		STATSLLINE			= 241;
	const int		STATSUPTIME			= 242;
	const int		STATSOLINE			= 243;
	const int		STATSHLINE			= 244;
	const int		UMODEIS				= 221;
	const int		LUSERCLIENT			= 251;
	const int		LUSEROP				= 252;
	const int		LUSERUNKNOWN		= 253;
	const int		LUSERCHANNELS		= 254;
	const int		LUSERME				= 255;
	const int		ADMINME				= 256;
	const int		ADMINLOC1			= 257;
	const int		ADMINLOC2			= 258;
	const int		ADMINEMAIL			= 259;
}

class Reply
{
private:
	std::string					_msg;
public:
	Reply(const int num, std::vector<std::string> param = std::vector<std::string>());
	Reply(const Reply &ref);
	Reply &operator=(const Reply &ref);
	~Reply();
public:
	const std::string &get_msg();
};

#endif