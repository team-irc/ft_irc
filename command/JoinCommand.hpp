#ifndef JOIN_COMMAND_HPP
# define JOIN_COMMAND_HPP

# include "Command.hpp"

/*
Command: JOIN
Parameters: <channel>{,<channel>} [<key>{,<key>}]

JOIN 명령은 클라이언트가 특정 채널 청취를 시작하는 데 사용됩니다. 
클라이언트가 채널에 참여할 수 있는지 여부는 클라이언트가 연결된 서버에서만 확인됩니다. 
다른 모든 서버는 사용자가 다른 서버에서 수신되면 자동으로 사용자를 채널에 추가합니다. 

이에 영향을 미치는 조건은 다음과 같습니다.

1. 채널이 초대 전용 인 경우 사용자를 초대해야합니다.
2. 사용자의 별명 / 사용자 이름 / 호스트 이름은 활성 차단과 일치하지 않아야합니다.
3. 설정된 경우 올바른 키 (암호)를 제공해야합니다.

이것들은 MODE 명령 아래에서 더 자세히 논의됩니다 (자세한 내용은 4.2.3 절 참조).
사용자가 채널에 가입하면 채널에 영향을 미치는 서버가 수신하는 모든 명령에 대한 알림을받습니다. 
여기에는 MODE, KICK, PART, QUIT 및 물론 PRIVMSG / NOTICE가 포함됩니다. 
JOIN 명령은 각 서버가 채널에있는 사용자를 찾을 수있는 위치를 알 수 있도록 모든 서버에 브로드 캐스트되어야합니다. 
이를 통해 PRIVMSG / NOTICE 메시지를 채널에 최적으로 전달할 수 있습니다.

JOIN이 성공하면 사용자에게 채널의 주제 (RPL_TOPIC 사용)와 채널에있는 사용자 목록 (RPL_NAMREPLY 사용)이 전송되며 여기에는 사용자 참여가 포함되어야합니다.

Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
           ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
           ERR_CHANNELISFULL               ERR_BADCHANMASK
           ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
           RPL_TOPIC

Examples:

JOIN #foobar                    ; join channel #foobar.

JOIN &foo fubar                 ; join channel &foo using key "fubar".

JOIN #foo,&bar fubar            ; join channel #foo using key "fubar"
								and &bar using no key.

JOIN #foo,#bar fubar,foobar     ; join channel #foo using key "fubar".
								and channel #bar using key "foobar".

JOIN #foo,#bar                  ; join channels #foo and #bar.

:WiZ JOIN #Twilight_zone        ; JOIN message from WiZ
*/

class JoinCommand : public Command
{
private:

public:
	JoinCommand();
	virtual ~JoinCommand();
	JoinCommand(JoinCommand const &copy);
	JoinCommand	&operator=(JoinCommand const &ref);

	void	run(IrcServer &irc);
private:
	bool deal_exception(IrcServer &irc);
};

#endif