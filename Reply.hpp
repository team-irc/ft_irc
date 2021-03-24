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
	const int		NOSUCHCHANNEL		= 403; //
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
}

class Reply
{
private:
	static std::map<int, std::string> replies;
	std::string _msg;
	std::string nick;
	std::vector<std::string> param;
public:
	Reply();
	Reply(const int num);
	Reply(const Reply &ref);
	Reply &operator=(const Reply &ref);
	const std::string &get_msg();
};

#endif