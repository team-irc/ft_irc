#include "PrivmsgCommand.hpp"
#include "ft_irc.hpp"

PrivmsgCommand::PrivmsgCommand()
{}

PrivmsgCommand::PrivmsgCommand(const PrivmsgCommand &ref)
{}

PrivmsgCommand &PrivmsgCommand::operator=(const PrivmsgCommand &ref)
{
	return (*this);
}

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

void			PrivmsgCommand::send_member(IrcServer &irc, Member &member)
{
	Socket			*sock = irc.get_socket_set().find_socket(member.get_fd());

	add_prefix(irc);
	sock->write(_msg.get_msg());
	if (!member.get_away().empty() && irc.get_current_socket()->get_type() == CLIENT)
		irc.get_current_socket()->write(Reply(RPL::AWAY(), member.get_nick(), member.get_away()).get_msg().c_str());
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

void			PrivmsgCommand::send_channel(IrcServer &irc, Channel &channel)
{
	std::vector<ChanMember>				members = channel.get_members();
	std::vector<ChanMember>::iterator	begin = members.begin();
	std::vector<ChanMember>::iterator	end = members.end();
	int			fd;
	std::string	prefix;
	std::vector<int>					send_fd;

	add_prefix(irc);
	prefix = _msg.get_prefix().substr(0, _msg.get_prefix().find('!'));
	while (begin != end)
	{
		fd = (*begin)._member->get_fd();
		// 해당 메시지를 보낸 유저에는 메시지를 전송하지 않아야 함
		// 채널 내에 있을 수도 있고 없을 수도 있음(prefix로 구분?)
		if (((*begin)._member->get_nick() != prefix) &&
			((*begin)._member->get_fd() != _msg.get_source_fd()) &&
			(!isInside(send_fd, fd)))
			{
				(irc.get_socket_set().find_socket(fd))->write(_msg.get_msg());
				send_fd.push_back(fd);
			}
		begin++;
	}
}

void			PrivmsgCommand::check_receiver(IrcServer &irc, const std::string &recv)
{
	Channel			*channel = irc.get_channel(recv);
	Member			*member = irc.get_member(recv);

	if (channel)
		send_channel(irc, *channel);
	else if (member)
		send_member(irc, *member);
}

void			PrivmsgCommand::run(IrcServer &irc)
{
	std::string		*recvs;
	int				param_size;
	std::string		msg;
	std::string		err;

	if (_msg.get_param_size() == 0)
		throw (Reply(ERR::NORECIPIENT(), "PRIVMSG"));
	if (_msg.get_param_size() == 1 || _msg.get_param(1).empty())
		throw (Reply(ERR::NOTEXTTOSEND()));
	param_size = ft::split(_msg.get_param(0), ',', recvs);
	msg = _msg.get_param(1);
	for (int i = 0; i < param_size; i++)
		check_receiver(irc, recvs[i]);
	delete[] recvs;
}

// privmsg #hi hihi
// :irc.example.net 401 a #hi :No such nick or channel name
// nick 11
// :irc.example.net 432 a 11 :Erroneous nickname