#include "NoticeCommand.hpp"
#include "ft_irc.hpp"

NoticeCommand::NoticeCommand()
{}

NoticeCommand::NoticeCommand(const NoticeCommand &ref)
{}

NoticeCommand &NoticeCommand::operator=(const NoticeCommand &ref)
{
	return (*this);
}

NoticeCommand::~NoticeCommand()
{}

void			NoticeCommand::add_prefix(IrcServer &irc)
{
	Member			*src_mem;
	std::string		msg;

	if (_msg.get_prefix().empty())
	{
		// :a!~a@localhost Notice b :hello
		src_mem = irc.find_member(_msg.get_source_fd());
		msg += src_mem->get_nick() + "!~" + src_mem->get_username() + "@" + src_mem->get_hostname();
		_msg.set_prefix(msg);
	}
}

void			NoticeCommand::send_member(IrcServer &irc, Member &member)
{
	Socket			*sock = irc.get_socket_set().find_socket(member.get_fd());

	add_prefix(irc);
	sock->write(_msg.get_msg());
}

void			NoticeCommand::send_channel(IrcServer &irc, Channel &channel)
{
	std::vector<ChanMember>	members = channel.get_members();
	std::vector<ChanMember>::iterator begin = members.begin();
	std::vector<ChanMember>::iterator end = members.end();
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
			((*begin)._member->get_fd() != _msg.get_source_fd()))
			(irc.get_socket_set().find_socket(fd))->write(_msg.get_msg());
		begin++;
	}
}

void			NoticeCommand::check_receiver(IrcServer &irc, const std::string &recv)
{
	Channel			*channel = irc.get_channel(recv);
	Member			*member = irc.get_member(recv);

	if (channel)
		send_channel(irc, *channel);
	else if (member)
		send_member(irc, *member);
}

void			NoticeCommand::run(IrcServer &irc)
{
	std::string		*recvs;
	int				param_size;
	std::string		msg;
	std::string		err;

	if (_msg.get_param_size() == 0)
		return ;
	if (_msg.get_param_size() == 1 || _msg.get_param(1).empty())
		return ;
	param_size = ft::split(_msg.get_param(0), ',', recvs);
	msg = _msg.get_param(1);
	for (int i = 0; i < param_size; i++)
		check_receiver(irc, recvs[i]);
	delete[] recvs;
}