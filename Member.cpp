#include "Member.hpp"

Member::Member()
{
};

Member::Member(const Member & other):
	_nick(other._nick), _username(other._username), _hostname(other._hostname),
	_servername(other._servername), _realname(other._realname)
{
};

Member::Member(const std::string &nick, const std::string &username,
	const std::string &hostname, const std::string &servername,
	const std::string &realname)
{
	_nick = nick;
	_username = username;
	_hostname = hostname;
	_servername = servername;
	_realname = realname;
};

Member & Member::operator = (const Member & other)
{
	_nick = other._nick;
	_username = other._username;
	_hostname = other._hostname;
	_servername = other._servername;
	_realname = other._realname;
	return (*this);
};

Member::~Member()
{
};

const bool			Member::is_setting() const
{
	if (!_nick.empty() && !_username.empty() && !_hostname.empty()
		&& !_servername.empty() && !_realname.empty())
		return (true);
	return (false);
}

const std::string	&Member::get_nick() { return (_nick); }
void				Member::set_nick(const std::string &nick) { _nick = nick; }
const std::string	&Member::get_username() { return (_username); }
void				Member::set_username(const std::string &username) { _username = username; }
const std::string	&Member::get_hostname() { return (_hostname); }
void				Member::set_hostname(const std::string &hostname) { _hostname = hostname; }
const std::string	&Member::get_servername() { return (_servername); }
void				Member::set_servername(const std::string &servername) { _servername = servername; }
const std::string	&Member::get_realname() { return (_realname); }
void				Member::set_realname(const std::string &realname) { _realname = realname; }