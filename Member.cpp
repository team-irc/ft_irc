#include "Member.hpp"
#include "Channel.hpp"

Member::Member()
{
};

Member::Member(const Member & other):
	_nick(other._nick), _username(other._username), _hostname(other._hostname),
	_servername(other._servername), _realname(other._realname), _mode(0)
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
};

Member & Member::operator = (const Member & other)
{
	_nick = other._nick;
	_username = other._username;
	_hostname = other._hostname;
	_servername = other._servername;
	_realname = other._realname;
	_mode = other._mode;
	return (*this);
};

Member::~Member()
{
	std::set<Channel *>::iterator	joinned_channel;

	joinned_channel = _joinned_channels.begin();
	while (joinned_channel != _joinned_channels.end())
	{
		(*joinned_channel)->delete_member(this);
		joinned_channel++;
	}
};

bool				Member::add_channel(Channel *channel)
{
	return ((_joinned_channels.insert(channel)).second);
}

const bool			Member::is_setting() const
{
	if (!_nick.empty() && !_username.empty() && !_hostname.empty()
		&& !_servername.empty() && !_realname.empty())
	{
		if (std::isdigit(_nick.at(0)))
			return (false);
		return (true);
	}
	return (false);
}

const std::string	&Member::get_nick()										{ return (_nick); }
void				Member::set_nick(const std::string &nick)				{ _nick = nick; }
const std::string	&Member::get_username()									{ return (_username); }
void				Member::set_username(const std::string &username)		{ _username = username; }
const std::string	&Member::get_hostname()									{ return (_hostname); }
void				Member::set_hostname(const std::string &hostname)		{ _hostname = hostname; }
const std::string	&Member::get_servername()								{ return (_servername); }
void				Member::set_servername(const std::string &servername)	{ _servername = servername; }
const std::string	&Member::get_realname()									{ return (_realname); }
void				Member::set_realname(const std::string &realname)		{ _realname = realname; }
int					Member::get_fd()										{ return (_fd); }
void				Member::set_fd(int fd)									{ _fd = fd; }
std::set<Channel *>	Member::get_joinned_channels()							{ return (_joinned_channels); }

bool				Member::delete_channel(Channel *channel)
{
	if (_joinned_channels.find(channel) == _joinned_channels.end())
		return (false);
	else
	{
		_joinned_channels.erase(channel);
		return (true);
	}
}

char			Member::get_mode() { return (_mode); }
bool				Member::check_mode(char mode, bool is_set)
{
	if (is_set)
	{
		if (mode == 'i')
		{
			if (_mode & 8)
				return (false);
			return (true);
		}
		else if (mode == 'w')
		{
			if (_mode & 4)
				return (false);
			return (true);
		}
		else if (mode == 's')
		{
			if (_mode & 2)
				return (false);
			return (true);
		}
		else if (mode == 'o')
		{
			if (_mode & 1)
				return (false);
			return (true);
		}
	}
	else
	{
		if (mode == 'i')
		{
			if (_mode & 8)
				return (true);
			return (false);
		}
		else if (mode == 'w')
		{
			if (_mode & 4)
				return (true);
			return (false);
		}
		else if (mode == 's')
		{
			if (_mode & 2)
				return (true);
			return (false);
		}
		else if (mode == 'o')
		{
			if (_mode & 1)
				return (true);
			return (false);
		}
	}
	return (false);
}

void				Member::set_mode(char mode) { _mode = mode; }
