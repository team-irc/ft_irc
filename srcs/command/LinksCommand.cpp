#include "LinksCommand.hpp"

/*

	LINKS를 사용하면 쿼리에 응답하는 서버가 알고있는 모든 서버를 나열 할 수 있습니다. 
	반환 된 서버 목록은 마스크와 일치해야합니다. 그렇지 않으면 마스크가 제공되지 않으면 전체 목록이 반환됩니다.
	<server mask>와 함께 <remote server>가 제공되면 LINKS 명령이 해당 이름 (있는 경우)과 일치하는 
	첫 번째 서버로 전달되고 해당 서버는 쿼리에 응답해야합니다.


    Command: LINKS
       Parameters: [[<remote server>] <server mask>]

       With LINKS, a user can list all servers which are known by the server
       answering the query.  The returned list of servers must match the
       mask, or if no mask is given, the full list is returned.

       If <remote server> is given in addition to <server mask>, the LINKS
       command is forwarded to the first server found that matches that name
       (if any), and that server is then required to answer the query.

       Numeric Replies:

               ERR_NOSUCHSERVER
               RPL_LINKS                       RPL_ENDOFLINKS

       Examples:

    LINKS *.au                      ; list all servers which have a name
                                    that matches *.au;

    :WiZ LINKS *.bu.edu *.edu       ; LINKS message from WiZ to the first
                                    server matching *.edu for a list of
                                    servers matching *.bu.edu.
    
*/

static void		send_links_reply(IrcServer &irc, Socket *socket, std::string const &mask = "")
{
	Server										*server;
	std::string									server_name;
	std::map<std::string, Server *>::iterator	iter;

	iter = irc.get_global_server().begin();
	while (iter != irc.get_global_server().end()) // 연결된 서버들을 순회하면서, Reply를 전송
	{
		server_name = (*iter).first;
		server = (*iter).second;
		if (mask.empty()) // 마스크가 없다면 모두 보내고, 있다면 마스크 체크 후에 일치하는것만 전송
			socket->write(Reply(RPL::LINKS(), "", server->get_name(), ft::itos(server->get_hopcount()), server->get_info()).get_msg().c_str());
		else if (ft::check_mask(server_name, mask))
			socket->write(Reply(RPL::LINKS(), mask, server->get_name(), ft::itos(server->get_hopcount()), server->get_info()).get_msg().c_str());
		iter++;
	}
	socket->write(Reply(RPL::ENDOFLINKS(), mask).get_msg().c_str());
}

bool	LinksCommand::transfer_message(IrcServer &irc, std::string const &server_name, std::string const &mask = "")
{
	Socket	*socket;
	Server	*server;

	socket = irc.get_current_socket();
	if (server_name == irc.get_serverinfo().SERVER_NAME) // 나에게 요청한경우 (예외)
	{
		send_links_reply(irc, socket, mask);
		return (false);
	}
	_msg.set_prefix(irc.find_member(socket->get_fd())->get_nick()); // 1. prefix에 nickname 추가
	if (!(server = irc.get_server(server_name))) // 2. 해당 서버를 찾는다
		throw (Reply(ERR::NOSUCHSERVER(), _msg.get_param(0)));
	server->get_socket()->write(_msg.get_msg()); // 3. 해당 서버쪽으로 메세지 전송
	return (true);
}

static bool		is_mask(std::string const &str)
{
	if (str.find('*') == std::string::npos && str.find('?') == std::string::npos)
		return (false);
	else
		return (true);
}

void 	LinksCommand::run(IrcServer &irc)
{
	Socket			*socket;
	Server			*server;
	std::string		server_name;

	socket = irc.get_current_socket();
	if (socket->get_type() == CLIENT)
	{
		if (_msg.get_param_size() == 0) // Links만 온 경우
			send_links_reply(irc, socket, "");
		else if (_msg.get_param_size() == 1) // 마스크 또는 다른 서버주소만 온 경우
		{
			if (!is_mask(_msg.get_param(0))) 
			{
				transfer_message(irc, _msg.get_param(0), ""); // 마스크가 없고, 다른 서버주소만 파라미터로 주어진 경우
				return ;
			}
			else // 마스크라면, 마스크와 매칭되는 모든 서버에 대한 정보를 전송
				send_links_reply(irc, socket, _msg.get_param(0));
		}
		else if (_msg.get_param_size() == 2)
		{
			transfer_message(irc, _msg.get_param(0), _msg.get_param(1)); // 다른서버에 마스크와 함께 전송
			return ;
		}
	}
	else if (socket->get_type() == SERVER)
	{
		Reply::set_servername(irc.get_serverinfo().SERVER_NAME); // Reply 설정! 필수
		Reply::set_username(_msg.get_prefix());
		server_name = _msg.get_param(0);
		if (server_name == irc.get_serverinfo().SERVER_NAME) // 나에게 보낸거라면
		{
			if (_msg.get_param_size() == 1) // 마스크 없이 온 경우
				send_links_reply(irc, socket, "");
			else if (_msg.get_param_size() == 2) // 마스크까지 온 경우
				send_links_reply(irc, socket, _msg.get_param(1));
		}
		else // 내가 아니라면 해당 서버로 전송
		{
			server = irc.get_server(server_name);
			server->get_socket()->write(_msg.get_origin());
		}
	}
	if (socket->get_type() == UNKNOWN)
		throw(Reply(ERR::NOTREGISTERED()));
}

LinksCommand::LinksCommand(): Command()
{
}

LinksCommand::~LinksCommand()
{
}