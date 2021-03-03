#include "User.hpp"

User::User()
{
}

User::~User()
{
}

void	User::set_username(const std::string &username)
{
	_username = username;
}

void	User::set_nickname(const std::string &nickname)
{
	_nickname = nickname;
}

std::string		User::get_username()
{
	return (_username);
}

std::string		User::get_nickname()
{
	return (_nickname);
}