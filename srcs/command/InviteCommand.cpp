#include "InviteCommand.hpp"

/*
	INVITE 메시지는 사용자를 채널에 초대하는 데 사용됩니다. 
	매개 변수 <nickname>은 대상 채널 <channel>에 초대 될 사람의 별명입니다. 
	대상 사용자가 초대되는 채널이 존재하거나 유효한 채널이어야한다는 요구 사항은 없습니다. 
	초대 전용 (MODE + i) 채널에 사용자를 초대하려면 초대를 보내는 클라이언트가 해당 채널의 채널 운영자로 인식되어야합니다.

 
    Command: INVITE
       Parameters: <nickname> <channel>

       The INVITE message is used to invite users to a channel.  The
       parameter <nickname> is the nickname of the person to be invited to
       the target channel <channel>.  There is no requirement that the
       channel the target user is being invited to must exist or be a valid
       channel.  To invite a user to a channel which is invite only (MODE
       +i), the client sending the invite must be recognised as being a
       channel operator on the given channel.

       Numeric Replies:

               ERR_NEEDMOREPARAMS              ERR_NOSUCHNICK
               ERR_NOTONCHANNEL                ERR_USERONCHANNEL
               ERR_CHANOPRIVSNEEDED
               RPL_INVITING                    RPL_AWAY

       Examples:

       :Angel INVITE Wiz #Dust         ; User Angel inviting WiZ to channel
                                       #Dust

       INVITE Wiz #Twilight_Zone       ; Command to invite WiZ to
                                       #Twilight_zone

*/

void	InviteCommand::run(IrcServer &irc)
{
	Socket	*socket;
	Member	*member;
	Member	*invited_member;
	Channel	*channel;

	socket = irc.get_current_socket();
	if (socket->get_type() == UNKNOWN)
		throw (Reply(ERR::NOTREGISTERED()));
	if (socket->get_type() == CLIENT)
	{
		if (_msg.get_param_size() < 2)
			throw(Reply(ERR::NEEDMOREPARAMS(), _msg.get_command()));
		member = irc.find_member(socket->get_fd());
		_msg.set_prefix(member->get_nick());
		invited_member = irc.get_member(_msg.get_param(0));
		if (!invited_member)
			throw(Reply(ERR::NOSUCHNICK(), _msg.get_param(0)));
		channel = irc.get_channel(_msg.get_param(1));
		if (!channel)
			throw(Reply(ERR::NOSUCHCHANNEL(), _msg.get_param(1)));
		if (channel->find_member(member) == false)
			throw(Reply(ERR::NOTONCHANNEL(), channel->get_name()));
		if (channel->check_mode('i', 0) && !channel->is_operator(member)) // 채널 모드에 i가 포함되어 있고 명령자가 op가 아니라면
			throw(Reply(ERR::CHANOPRIVSNEEDED(), channel->get_name()));
		channel->add_invited_member(invited_member); // 초대 멤버 목록에 추가
		
		// 초대한애 한테 메세지 전송
		socket->write(Reply(RPL::INVITING(), channel->get_name(), invited_member->get_nick()).get_msg().c_str());
		
		// 초대받은애한테 메세지 전송
		if (invited_member->get_socket()->get_type() == CLIENT)
			invited_member->get_socket()->write(_msg.get_msg());

		// 다른 서버에 메세지 전파
		_msg.set_prefix(member->get_nick() + "!~" + member->get_username() + "@" + irc.get_serverinfo().SERVER_NAME);
		irc.send_msg_server(socket->get_fd(), _msg.get_msg());
	}
	else if (socket->get_type() == SERVER)
	{
		invited_member = irc.get_member(_msg.get_param(0));
		channel = irc.get_channel(_msg.get_param(1));
		channel->add_invited_member(invited_member);

		// 초대받은애한테 전송
		if (invited_member->get_socket()->get_type() == CLIENT)
			invited_member->get_socket()->write(_msg.get_msg());
		
		// 다른 서버에 메세지 전파
		irc.send_msg_server(socket->get_fd(), _msg.get_msg());
	}
	else if (socket->get_type() == UNKNOWN)
		throw(Reply(ERR::NOTREGISTERED()));
}

InviteCommand::InviteCommand() : Command()
{
}

InviteCommand::~InviteCommand()
{
}