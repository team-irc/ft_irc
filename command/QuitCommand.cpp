#include "QuitCommand.hpp"
#include "ft_irc.hpp"

void	QuitCommand::run(IrcServer &irc)
{
	Socket		*socket;
	Member		*member;

	socket = irc.get_current_socket();
	if (socket->get_type() == CLIENT)
	{
		member = irc.find_member(socket->get_fd()); // 1. 멤버를 찾는다.
		_msg.set_prefix(member->get_nick()); // 2. 메세지를 전파하기 위해 닉네임을 프리픽스로 설정
		irc.delete_member(member->get_nick()); // 3. 멤버를 _global_user에서 지운다.
		delete member;							// 멤버 인스턴스 삭제; 채널나가기
		irc.get_socket_set().remove_socket(socket);	// 소켓을 _global_socket에서 지운다.
		delete socket;// 4. 소켓 인스턴스 제거; close
		irc.send_msg_server(0, _msg.get_msg());// 5. 다른 서버에서도 _global_user에서 해당 유저를 지울 수 있도록 메세지를 보낸다.
	}
	else if (socket->get_type() == SERVER)
	{
		member = irc.get_member(_msg.get_prefix()); // 1. prefix로 멤버를 찾는다.
		irc.delete_member(member->get_nick()); // 2. 멤버를 _global_user에서 지운다.
		delete member;
		irc.send_msg_server(socket->get_fd(), _msg.get_msg()); // 3. 다른서버에도 메세지를 보낸다.
		// 소켓을 제거하지는 않는다. 서버에서 전송된 메세지이므로
	}
	else if (socket->get_type() == UNKNOWN)
	{
		member = irc.find_member(socket->get_fd()); // 1. fd값으로 멤버를 찾는다.
		if (member)
		{
			irc.delete_member(std::to_string(socket->get_fd())); // 2. 멤버를 _global_user에서 지운다.
			delete member; //멤버 인스턴스를 제거
		}
		irc.get_socket_set().remove_socket(socket);
		delete socket; // 3. 소켓 연결을 끊는다. 다른 서버에는 전송하지 않는다. 다른서버에는 이 멤버가 등록되지 않은 상태이므로
	}
}

QuitCommand::QuitCommand() : Command()
{
}

QuitCommand::~QuitCommand()
{
}

QuitCommand::QuitCommand(QuitCommand const &copy)
{
	_msg = copy._msg;
}

QuitCommand	&QuitCommand::operator=(QuitCommand const &ref)
{
	_msg = ref._msg;
	return (*this);
}