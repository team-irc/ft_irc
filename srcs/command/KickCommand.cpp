#include "KickCommand.hpp"

/*
	KICK 명령을 사용하여 채널에서 사용자를 강제로 제거 할 수 있습니다. 
	채널에서 '추방'합니다 (강제 부분). 
	채널 운영자 만이 다른 사용자를 채널에서 퇴장시킬 수 있습니다. 
	KICK 메시지를 수신하는 각 서버는 해당 메시지가 유효한지 (즉, 보낸 사람이 실제로 채널 운영자 임) 채널에서 피해자를 제거하기 전에 확인합니다.

    Command: KICK
       Parameters: <channel> <user> [<comment>]

       The KICK command can be  used  to  forcibly  remove  a  user  from  a
       channel.   It  'kicks  them  out'  of the channel (forced PART).
       Only a channel operator may kick another user out of a  channel.
       Each  server that  receives  a KICK message checks that it is valid
       (ie the sender is actually a  channel  operator)  before  removing
       the  victim  from  the channel.

       Numeric Replies:

               ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
               ERR_BADCHANMASK                 ERR_CHANOPRIVSNEEDED
               ERR_NOTONCHANNEL

       Examples:

    KICK &Melbourne Matthew         ; Kick Matthew from &Melbourne

    KICK #Finnish John :Speaking English
                                    ; Kick John from #Finnish using
                                    "Speaking English" as the reason
                                    (comment).

    :WiZ KICK #Finnish John         ; KICK message from WiZ to remove John
                                    from channel #Finnish

    NOTE:
         It is possible to extend the KICK command parameters to the

    following:

    <channel>{,<channel>} <user>{,<user>} [<comment>]
*/

void			KickCommand::add_prefix(IrcServer &irc)
{
	Member			*src_mem;
	std::string		msg;

	if (_msg.get_prefix().empty())
	{
		// :t1!~a@121.135.181.42 KICK #abc t3 :t1
		src_mem = irc.find_member(_msg.get_source_fd());
		msg += src_mem->get_nick() + "!~" + src_mem->get_username() + "@" + src_mem->get_hostname();
		_msg.set_prefix(msg);
	}
}

void KickCommand::run(IrcServer &irc)
{
	Socket			*socket;
	Member			*member;
	Member			*target_member;
	std::string		target_member_name;
	std::string		channel_name;	
	Channel			*channel;


	socket = irc.get_current_socket();
	if (socket->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
	if (socket->get_type() == CLIENT)
	{
		if (!(_msg.get_param_size() >= 2 && _msg.get_param_size() <= 3))
			throw(Reply(ERR::NEEDMOREPARAMS(), _msg.get_command()));
		member = irc.find_member(socket->get_fd());

		channel_name = _msg.get_param(0);
		target_member_name = _msg.get_param(1);

		target_member = irc.get_member(target_member_name);
		if (!target_member)					// 타겟 멤버가 없는경우
			throw(Reply(ERR::NOSUCHNICK(), target_member_name));

		channel = irc.get_channel(channel_name);
		if (!channel) 						// 그런 채널이 없는경우
			throw(Reply(ERR::NOSUCHCHANNEL(), channel_name));

		if (!channel->find_member(member))	// 멤버가 채널에 없는경우
			throw(Reply(ERR::NOTONCHANNEL(), channel->get_name()));

		if (!channel->find_member(member))	// 타겟 멤버가 채널에 없는경우
			throw(Reply(ERR::NOSUCHNICK(), target_member_name));

		if (!channel->is_operator(member))	// kick 명령자가 채널 오퍼레이터가 아닌 경우
			throw(Reply(ERR::CHANOPRIVSNEEDED(), channel->get_name()));
		
		// 다른 서버에 메세지 전파
		add_prefix(irc);
		if (_msg.get_param_size() == 2)
		{
			std::string		trailing;
			trailing = ":" + member->get_nick();
			_msg.set_param_at(2, trailing);
		}
		irc.send_msg_server(socket->get_fd(), _msg.get_msg());
		channel->send_msg_to_members(_msg.get_msg()); // 채널에 있는 유저들에게도 전송

		channel->delete_member(target_member); // kick 당할 멤버에게도 메세지가 전달 된 후 제거되어야함
		target_member->delete_channel(channel);
	}
	else if (socket->get_type() == SERVER)
	{
		target_member_name = _msg.get_param(1);
		channel_name = _msg.get_param(0);

		target_member = irc.get_member(target_member_name);
		channel = irc.get_channel(channel_name);
		
		irc.send_msg_server(socket->get_fd(), _msg.get_msg());
		channel->send_msg_to_members(_msg.get_msg());

		channel->delete_member(target_member);
		target_member->delete_channel(channel);
	}
	else if (socket->get_type() == UNKNOWN)
		throw(Reply(ERR::NOTREGISTERED()));
}

KickCommand::KickCommand(): Command()
{
}

KickCommand::~KickCommand()
{
}