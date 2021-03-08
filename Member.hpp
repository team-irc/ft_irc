#ifndef MEMBER_HPP
#define MEMBER_HPP

#include <string>

class Member
{
private:
	std::string		_nick;
	std::string		_username;
	std::string		_hostname;
	std::string		_servername;
	std::string		_realname;
public:
	Member();
	Member::Member(const std::string &nick, const std::string &username,
		const std::string &hostname, const std::string &servername,
		const std::string &realname);
	Member(const Member & other);
	Member & operator = (const Member & other);
	virtual ~Member();
public:
	const std::string	&get_nick();
	void				set_nick(const std::string &nick);
	const std::string	&get_username();
	void				set_username(const std::string &username);
	const std::string	&get_hostname();
	void				set_hostname(const std::string &hostname);
	const std::string	&get_servername();
	void				set_servername(const std::string &servername);
	const std::string	&get_realname();
	void				set_realname(const std::string &realname);
	const bool			is_setting() const;
};

#endif