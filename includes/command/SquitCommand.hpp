#ifndef SQUIT_COMMAND_HPP
# define SQUIT_COMMAND_HPP

# include "Command.hpp"

/*
1. Command: SQUIT
2. Parameters: <server> <comment>
3. Replies
    1. ERR_NOPRIVILEGES
    2. ERR_NOSUCHSERVER
서버 종료 또는 작동 중지에 대해 알려주는 데 필요
다른 서버와의 연결을 끊으려면 다른 서버의 이름을 서버 매개 변수로 사용하여 다른 서버에 SQUIT 메시지를 보내야 함
<comment>는 (현재 서버에 연결되지 않은)원격 서버에 대해 SQUIT를 실행하는 모든 운영자가 제공해야 다른 운영자가 이 작업의 이유를 알 수 있음
닫히는 연결의 양쪽에 있는 두 서버는 해당 링크 뒤에 있는 것으로 간주되는 다른 모든 서버에 대해 SQUIT 메시지를 보내야 함
마찬가지로 QUIT 메시지는 해당 링크 뒤에 있는 모든 클라이언트를 대신하여 나머지 네트워크에 연결된 다른 서버로 보내야 함
분할로 인해 멤버를 잃은 채널의 모든 채널 멤버에게 QUIT 메시지를 보내야 함
*/

class SquitCommand : public Command
{
private:
	void	delete_server(int fd, IrcServer &irc);
	void	send_quit_user(int fd, IrcServer &irc);
	void	delete_connected_server(IrcServer &irc, Server *server);
public:
	SquitCommand();
	~SquitCommand();
public:
	void	run(IrcServer &irc);
};

#endif