#include "JoinCommand.hpp"

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

/*
** 처리 안함 리스트
** ERR_BADCHANNELKEY
** ERR_BADCHANMASK
*/

static void		send_join_reply(Socket *socket, Channel *channel)
{
	std::string		join_msg;

	join_msg = ":" + socket->get_linkname() + " JOIN " + channel->get_name() + "\n";
	channel->send_msg_to_members(join_msg.c_str());
	if (!channel->get_topic().empty())
		socket->write(Reply(RPL::TOPIC(), channel->get_name(), channel->get_topic()));
	socket->write(Reply(RPL::NAMREPLY(), channel->get_name(), channel->get_member_list()));
	socket->write(Reply(RPL::ENDOFNAMES(), channel->get_name()));
}

int		JoinCommand::join(IrcServer &irc, Member *member, std::string const &channel_name, std::string const &channel_key = "")
{
	std::string		msg;
	Channel			*channel;
	Socket			*socket;

	channel = irc.get_channel(channel_name);
	socket = irc.get_current_socket();
	if (channel == NULL)
	{
		channel = new Channel(channel_name, member, channel_key);
		irc.add_channel(channel->get_name(), channel);
		channel->add_operator(member);
		if (!channel_key.empty())
			channel->set_key(channel_key);
		member->add_channel(channel);
		channel->set_servername(irc.get_serverinfo().SERVER_NAME);

		// 클라이언트에 전송
		send_join_reply(socket, channel);

		// 서버에 전송
		if (channel_key.empty())
			msg = ":" + member->get_nick() + " JOIN " + channel->get_name() + "\n";
		else
			msg = ":" + member->get_nick() + " JOIN " + channel->get_name() + " " + channel_key + "\n";
		irc.send_msg_server(socket->get_fd(), msg.c_str());
		msg = ":" + member->get_nick() + " MODE " + channel->get_name() + " +o " + member->get_nick() + "\n";
		irc.send_msg_server(socket->get_fd(), msg.c_str());

	}
	else
	{
		if (channel->is_member(member))
			throw (Reply(ERR::USERONCHANNEL(), member->get_nick(), channel->get_name()));
		if (member->get_joined_channels().size() > MEMBER_CONST::JOINED_CHANNEL_LIMIT)
			throw (Reply(ERR::TOOMANYCHANNELS(), channel->get_name()));
		if (channel->is_ban_list(member->get_nick()))
			throw (Reply(ERR::BANNEDFROMCHAN(), channel->get_name()));
		if (channel->get_members().size() >= channel->get_limit())
			throw (Reply(ERR::CHANNELISFULL(), channel->get_name()));
		if (channel->check_mode('i', 0) && !channel->is_invited_member(member))
			throw (Reply(ERR::INVITEONLYCHAN(), channel->get_name()));
		if (!channel->is_valid_key(channel_key))
			throw (Reply(ERR::BADCHANNELKEY(), channel->get_name()));
		else
		{
			member->add_channel(channel);
			channel->add_member(member);
			send_join_reply(socket, channel); // 클라이언트에 전송
			msg = ":" + member->get_nick() + " JOIN " + channel->get_name() + "\n"; // 서버에 전송
			irc.send_msg_server(socket->get_fd(), msg.c_str());
		}
	}
	return (0);
}

void	JoinCommand::run(IrcServer &irc)
{
	int				number_of_channel;
	int				number_of_key;
	std::string		*channel_names = 0;
	std::string		*channel_keys = 0;
	Socket			*socket;
	Member			*member;

	socket = irc.get_current_socket();
	if (socket->get_type() == CLIENT)
	{
		if (_msg.get_param_size() < 1)
			throw (Reply(ERR::NEEDMOREPARAMS(), "JOIN"));
		if ((_msg.get_param(0)[0] != '&') && (_msg.get_param(0)[0] != '#') && (_msg.get_param(0)[0] != '+'))
			throw (Reply(ERR::NOSUCHCHANNEL(), _msg.get_param(0)));
		number_of_channel = ft::split(_msg.get_param(0), ',', channel_names);
		if (_msg.get_param_size() == 2)
			number_of_key = ft::split(_msg.get_param(1), ',', channel_keys);
		member = irc.find_member(socket->get_fd());
		for (int i = 0; i < number_of_channel; i++)
		{
			if (i < number_of_key)
				join(irc, member, channel_names[i], channel_keys[i]);
			else
				join(irc, member, channel_names[i]);
		}
		delete[] channel_names;
		if (channel_keys)
			delete[] channel_keys;
	}
	else if (socket->get_type() == SERVER)
	{
		// :t1 join #a (서버측으론 ,로 넘어오지 않는다.)
		std::string		channel_name;
		std::string		channel_key;
		Channel			*channel;

		channel_name = _msg.get_param(0);
		if (_msg.get_param_size() == 2)
			channel_key = _msg.get_param(1);
		member = irc.get_member(_msg.get_prefix());
		channel = irc.get_channel(channel_name);
		if (channel == 0) // 채널 새로 생성시
		{
			channel = new Channel(channel_name, member, channel_key);
			irc.add_channel(channel->get_name(), channel);
			member->add_channel(channel);
		}
		else // 기존 채널 참여시
		{
			channel->send_msg_to_members(_msg.get_msg());
			member->add_channel(channel);
			channel->add_member(member);
		}
		irc.send_msg_server(socket->get_fd(), _msg.get_msg());
	}
	else if (socket->get_type() == UNKNOWN)
		throw(Reply(ERR::NOTREGISTERED()));
}

JoinCommand::JoinCommand() : Command()
{
}

JoinCommand::~JoinCommand()
{
}