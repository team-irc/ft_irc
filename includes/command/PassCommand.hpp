#ifndef PASS_COMMAND_HPP
# define PASS_COMMAND_HPP

# include "Command.hpp"

// NICK, USER보다 먼저 보내져야 함
// pass 이전에 nick/user가 보내지고 PASS 명령어 실행 시 다음과 같은 오류 반환
// :irc.example.net 462 test :Connection already registered
// PASS 이전에 SERVER 들어온 경우 Syntax Error 반환
// 이후 PASS 입력 - server 입력을 하면 정상적으로 등록 됨

class PassCommand : public Command
{
public:
	PassCommand();
	~PassCommand();
public:
	void	run(IrcServer &irc);
};

#endif