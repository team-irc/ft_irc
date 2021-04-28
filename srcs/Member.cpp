#include "Member.hpp"
#include "Channel.hpp"

Member::Member(): _mode(0)
{
};

Member::Member(const Member & other):
	_nick(other._nick), _username(other._username), _hostname(other._hostname),
	_servername(other._servername), _realname(other._realname), _mode(0), _hopcount(0)
{
};

Member::Member(const std::string &nick, const std::string &username,
	const std::string &hostname, const std::string &servername,
	const std::string &realname, char mode)
{
	_nick = nick;
	_username = username;
	_hostname = hostname;
	_servername = servername;
	_realname = realname;
	_mode = mode;
	_hopcount = 0;
	time(&_last_action);
};

Member & Member::operator = (const Member & other)
{
	_nick = other._nick;
	_username = other._username;
	_hostname = other._hostname;
	_servername = other._servername;
	_realname = other._realname;
	_mode = other._mode;
	_hopcount = 0;
	return (*this);
};

Member::~Member()
{
};

bool				Member::add_channel(Channel *channel)
{
	return ((_joined_channels.insert(channel)).second);
}

bool				Member::delete_channel(Channel *channel)
{
	if (_joined_channels.find(channel) == _joined_channels.end())
		return (false);
	else
	{
		_joined_channels.erase(channel);
		return (true);
	}
}

bool				Member::check_mode(char mode, bool is_set)
{
	if (mode == 'i')
	{
		if (_mode & 8)
			return (!is_set);
		return (is_set);
	}
	else if (mode == 'w')
	{
		if (_mode & 4)
			return (!is_set);
		return (is_set);
	}
	else if (mode == 's')
	{
		if (_mode & 2)
			return (!is_set);
		return (is_set);
	}
	else if (mode == 'o')
	{
		if (_mode & 1)
			return (!is_set);
		return (is_set);
	}
	return (false);
}

/*
** GETTER
*/

const std::string	&Member::get_nick()										{ return (_nick); }
const std::string	&Member::get_username()									{ return (_username); }
const std::string	&Member::get_hostname()									{ return (_hostname); }
const std::string	&Member::get_servername()								{ return (_servername); }
const std::string	&Member::get_realname()									{ return (_realname); }
int					Member::get_fd()										{ return (_fd); }
std::set<Channel *>	&Member::get_joined_channels()							{ return (_joined_channels); }
const std::string	&Member::get_away()										{ return (_away); }
int					Member::get_hopcount()									{ return (_hopcount); }
Socket				*Member::get_socket()									{ return (_socket); }
char				Member::get_mode()										{ return (_mode); }
const std::string	&Member::get_mode_str()
{
	_mode_str = "+";
	if (check_mode('i', false))
		_mode_str += "i";
	if (check_mode('s', false))
		_mode_str += "s";
	if (check_mode('w', false))
		_mode_str += "w";
	if (check_mode('o', false))
		_mode_str += "o";
	return (_mode_str);
}

/*
** SETTER
*/

void				Member::set_nick(const std::string &nick)				{ _nick = nick; }
void				Member::set_username(const std::string &username)		{ _username = username; }
void				Member::set_hostname(const std::string &hostname)		{ _hostname = hostname; }
void				Member::set_servername(const std::string &servername)	{ _servername = servername; }
void				Member::set_realname(const std::string &realname)		{ _realname = realname; }
void				Member::set_fd(int fd)									{ _fd = fd; }
void				Member::set_away(const std::string &away)				{ _away = away; }
void				Member::set_hopcount(int cnt)							{ _hopcount = cnt; }
void				Member::set_socket(Socket *socket)						{ _socket = socket; }
void				Member::set_mode(char mode)								{ _mode = mode; }
