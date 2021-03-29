#include "SquitCommand.hpp"
#include "ft_irc.hpp"

void			find_squit_server(IrcServer &irc, int fd)
{
	std::map<std::string, int>::iterator	begin = _fd_map.begin();
	std::map<std::string, int>::iterator	end = _fd_map.end();

	while (begin != end)
	{
		if (begin->second == fd)
		{
			irc.delete_fd_map(begin->first);
		}
		begin++;
	}
}

void	SquitCommand::run(IrcServer &irc)
{
	Socket				*sock = irc.get_current_socket();
	std::string			servername;
	int					fd = irc.get_current_socket()->get_fd();

	// SQUIT은 서버에서 보내는 메시지라서 다른 타입이면 걍 무시
	if (sock->get_type() == SERVER)
	{
		if (_msg.get_prefix().empty()) // 직접 연결된 서버에서 SQUIT 하는 경우
		{
			// fd_map과 socketset에서 둘 다 지워야 함
			irc.get_socket_set().remove_socket(sock);
			delete sock;
														// 1. 해당 fd를 가진 모든 서버를 fd_map에서 제거 (+ 유저맵에서 제거)
														// 2. 해당 소켓 제거
														// 3. 다른 서버에 프리픽스 추가해서 SQUIT 전송
														// (4. 다른 서버에 유저 QUIT 전송?)
		}
		servername = _msg.get_param(0);
		irc.delete_fd_map(servername);
		_msg.set_prefix(irc.get_servername());
		irc.send_msg_server(fd, _msg.c_str());
		else // 직접 연결되지 않은 서버가 SQUIT 한 경우
		{

		}
	}
}

SquitCommand::SquitCommand() : Command()
{
}

SquitCommand::~SquitCommand()
{
}

SquitCommand::SquitCommand(SquitCommand const &copy)
{
	_msg = copy._msg;
}

SquitCommand	&SquitCommand::operator=(SquitCommand const &ref)
{
	_msg = ref._msg;
	return (*this);
}