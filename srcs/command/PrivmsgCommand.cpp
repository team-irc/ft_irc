#include "PrivmsgCommand.hpp"

PrivmsgCommand::PrivmsgCommand(): Command()
{}

PrivmsgCommand::~PrivmsgCommand()
{}

void			PrivmsgCommand::add_prefix(IrcServer &irc)
{
	Member			*src_mem;
	std::string		msg;

	if (_msg.get_prefix().empty())
	{
		// :a!~a@localhost PRIVMSG b :hello
		src_mem = irc.find_member(_msg.get_source_fd());
		msg += src_mem->get_nick() + "!~" + src_mem->get_username() + "@" + src_mem->get_hostname();
		_msg.set_prefix(msg);
	}
}

static bool		isInside(std::vector<std::string> const &vec, std::string const &val)
{
	std::vector<std::string>::const_iterator	begin = vec.begin();
	std::vector<std::string>::const_iterator	end = vec.end();

	while (begin != end)
	{
		if (*begin == val)
			return (true);
		begin++;
	}
	return (false);
}

void			PrivmsgCommand::send_member(IrcServer &irc, Member &member,
	std::vector<std::string> &send_nick)
{
	Socket			*sock = irc.get_socket_set().find_socket(member.get_fd());

	if (!isInside(send_nick, member.get_nick()))
	{
		add_prefix(irc);
		_msg.set_param_at(0, member.get_nick());
		sock->write(_msg.get_msg());
		if (!member.get_away().empty() && irc.get_current_socket()->get_type() == CLIENT)
			irc.get_current_socket()->write(Reply(RPL::AWAY(), member.get_nick(), member.get_away()).get_msg().c_str());
		send_nick.push_back(member.get_nick());
	}
}

static bool		isInside(std::vector<int> const &vec, int val)
{
	std::vector<int>::const_iterator	begin = vec.begin();
	std::vector<int>::const_iterator	end = vec.end();

	while (begin != end)
	{
		if (*begin == val)
			return (true);
		begin++;
	}
	return (false);
}

void			PrivmsgCommand::send_channel(IrcServer &irc, Channel &channel, Member *sender,
	std::vector<int> &send_channel_fd)
{
	std::vector<ChanMember>				members = channel.get_members();
	std::vector<ChanMember>::iterator	begin = members.begin();
	std::vector<ChanMember>::iterator	end = members.end();
	int			fd;
	std::string	prefix;

	add_prefix(irc);
	prefix = _msg.get_prefix().substr(0, _msg.get_prefix().find('!'));
	while (begin != end)
	{
		fd = (*begin)._member->get_fd();
		// 해당 메시지를 보낸 유저에는 메시지를 전송하지 않아야 함
		// 채널 내에 있을 수도 있고 없을 수도 있음(prefix로 구분?)
		if (((*begin)._member->get_nick() != prefix) &&
			((*begin)._member->get_fd() != _msg.get_source_fd()) &&
			(!isInside(send_channel_fd, fd)))
			{
				// 해당 채널의 모드를 확인하고 그에 맞는 동작 처리
				// n이 설정되었다면 sender가 해당 채널에 속했는지 확인
				// m이 설정된 상태면 sender가 해당 채널의 operator, creator, voice 권한이 있는지 확인
				if (channel.check_mode('n', false) && !channel.is_member(sender))
				{
					irc.get_current_socket()->write((Reply(ERR::CANNOTSENDTOCHAN(), channel.get_name())));
					return ;
				}
				if (channel.check_mode('m', false) && !(channel.is_voice(sender) || channel.is_operator(sender)))
				{
					irc.get_current_socket()->write((Reply(ERR::CANNOTSENDTOCHAN(), channel.get_name())));
					return ;
				}
				_msg.set_param_at(0, channel.get_name());
				(irc.get_socket_set().find_socket(fd))->write(_msg.get_msg());
				send_channel_fd.push_back(fd);
			}
		begin++;
	}
}

void			PrivmsgCommand::check_receiver(IrcServer &irc, const std::string &recv,
	std::vector<std::string> &send_nick, std::vector<int> &send_channel_fd)
{
	Channel						*channel = irc.get_channel(recv);
	Member						*member = irc.get_member(recv);
	Member						*sender = irc.find_member(irc.get_current_socket()->get_fd());

	if (channel)
		send_channel(irc, *channel, sender, send_channel_fd);
	else if (member)
		send_member(irc, *member, send_nick);
	else
	{
		if (ft::strchr(recv.c_str(), '*') || ft::strchr(recv.c_str(), '?'))
		{
			// 운영자 권한 확인
			// 없으면 noprivileged
			// 있으면 도메인쪽 확인 필요
			// '.'이 있는가, 있다면 그 이후로 와일드카드가 있는가
			// 제일 앞부분이 #인가 $인가에 따라 채널/서버 나눔
			// 서버의 경우 member의 서버 부분 확인 후 전송
			if (sender->check_mode('o', true))
			{
				irc.get_current_socket()->write(Reply(ERR::NOPRIVILEGES()));
				return ;
			}
			size_t		domain_idx = recv.find_last_of('.');
			if (domain_idx == std::string::npos)
			{
				irc.get_current_socket()->write(Reply(ERR::NOTOPLEVEL(), recv));
				return ;
			}
			std::string	domain = recv.substr(domain_idx + 1);
			if (ft::strchr(domain.c_str(), '*') || ft::strchr(domain.c_str(), '?'))
			{
				irc.get_current_socket()->write(Reply(ERR::WILDTOPLEVEL(), recv));
				return ;
			}
			if (recv.at(0) == '#')
			{
				// channel
				std::map<std::string, Channel *>::iterator begin = irc.get_global_channel().begin();
				std::map<std::string, Channel *>::iterator end = irc.get_global_channel().end();

				while (begin != end)
				{
					if (ft::check_mask(begin->second->get_name(), recv))
						send_channel(irc, *(begin->second), sender, send_channel_fd);
					begin++;
				}
			}
			else if (recv.at(0) == '$')
			{
				// 해당 서버를 가지고 있는 멤버들
				std::map<std::string, Member *>::iterator begin = irc.get_global_user().begin();
				std::map<std::string, Member *>::iterator end = irc.get_global_user().end();

				while (begin != end)
				{
					if (ft::check_mask(begin->second->get_servername(), recv.substr(1)))
						send_member(irc, *(begin->second), send_nick);
					begin++;
				}
			}
			else
			{
				irc.get_current_socket()->write(Reply(ERR::NOSUCHNICK(), recv));
				return ;
			}
		}
		else
		{
			// 서버한테 보내는거 아니면 nosuchnick
			// 일치하는 서버가 없으면 nosuchnick, 있으면 운영자 권한 확인 후 전송(이 시점에선 와일드카드 없음)
			if (!recv.empty() && recv.at(0) == '$')
			{
				Server	*server = irc.get_server(recv.substr(1));
				if (server)
				{
					if (sender->check_mode('o', true))
						throw (Reply(ERR::NOPRIVILEGES()));
					std::map<std::string, Member *>::iterator begin = irc.get_global_user().begin();
					std::map<std::string, Member *>::iterator end = irc.get_global_user().end();
					while (begin != end)
					{
						if (ft::check_mask(begin->second->get_servername(), recv.substr(1)))
							send_member(irc, *(begin->second), send_nick);
						begin++;
					}
				}
				else
				{
					irc.get_current_socket()->write(Reply(ERR::NOSUCHNICK(), recv));
					return ;
				}
			}
			else
			{
				irc.get_current_socket()->write(Reply(ERR::NOSUCHNICK(), recv));
				return ;
			}
		}
	}
}

void			PrivmsgCommand::run(IrcServer &irc)
{
	std::string					*recvs;
	int							param_size;
	std::string					msg;
	std::string					err;
	std::vector<int>			send_channel_fd;
	std::vector<std::string>	send_nick;

	if (irc.get_current_socket()->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
	if (_msg.get_param_size() == 0)
		throw (Reply(ERR::NORECIPIENT(), "PRIVMSG"));
	if (_msg.get_param_size() == 1 || _msg.get_param(1).empty())
		throw (Reply(ERR::NOTEXTTOSEND()));
	param_size = ft::split(_msg.get_param(0), ',', recvs);
	msg = _msg.get_param(1);
	for (int i = 0; i < param_size; i++)
		check_receiver(irc, recvs[i], send_nick, send_channel_fd);
	delete[] recvs;
}

// privmsg #hi hihi
// :irc.example.net 401 a #hi :No such nick or channel name
// nick 11
// :irc.example.net 432 a 11 :Erroneous nickname