#ifndef MEMBER_HPP
#define MEMBER_HPP

#include <string>
#include <set>
#include "Socket.hpp"
class Channel;

namespace MEMBER_CONST
{
	const size_t	JOINED_CHANNEL_LIMIT = 10;
}

class Member
{
private:
	int						_fd; // deprecated
	Socket					*_socket;
	std::string				_nick;
	std::string				_username;
	std::string				_hostname;
	std::string				_servername;
	std::string				_realname;
	std::set<Channel *>		_joinned_channels;
	std::string				_away;

	// mode i: mark a users as invisible;
	// mode s: marks a user for receipt of server notices;
	// mode w: user receives wallops;
	// mode o: operator flag;
	char			_mode;
	// mode o의 경우는 해당 채널에 있는 유저만 지정할 수 있음
	// mode o는 설정되더라도 :irc.example.net 379 test test :is using modes + 해당 위치에 나타나지 않음
	// 하나의 유저가 여러 서버를 가질 수 있음
public:
	Member();
	Member(const std::string &nick, const std::string &username,
		const std::string &hostname, const std::string &servername,
		const std::string &realname, char mode);
	Member(const Member & other);
	Member & operator = (const Member & other);
	~Member();
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
	int					get_fd();
	void				set_fd(int fd);
	Socket				*get_socket();
	void				set_socket(Socket *socket);
	char				get_mode();
	void				set_mode(char mode);
	const std::string	&get_away();
	void				set_away(const std::string &away);
	std::set<Channel *>	get_joinned_channels();
	bool				check_mode(char mode, bool is_set);
	const bool			is_setting() const;
	bool				add_channel(Channel *channel);
	bool				delete_channel(Channel *channel);
};

#endif