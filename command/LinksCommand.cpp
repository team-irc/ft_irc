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

// 연결된 서버에 대해서 알고있는 정보
// 1. 서버의 이름
// 2. 연결된 fd (_fd_map)
// 3. 

void 	LinksCommand::run(IrcServer &irc)
{
	Socket			*socket;
	Server			*server;
	std::string		server_name;
	std::string		mask;

	socket = irc.get_current_socket();
	if (socket->get_type() == CLIENT)
	{
		// 알고있는 모든 서버를 나열.
		std::map<std::string, Server *>::iterator	iter = irc.get_global_server().begin();
		// case 1. 파라미터가 없는 경우
		// case 2. 서버주소만 온 경우
		// case 3. 서버 마스크만 온 경우
		// case 4. 서버 주소와 서버 마스크가 온 경우
		if (_msg.get_param_size() == 0)
		{
			while (iter != irc.get_global_server().end())
			{
				server = (*iter).second;
				socket->write(Reply(RPL::LINKS(), "", server->get_name(), std::to_string(server->get_hopcount()), server->get_info()).get_msg().c_str());
				iter++;
			}
		}
		else if (_msg.get_param_size() == 1)
		{
			if (_msg.get_param(0).find('*') == std::string::npos)
			{ // 마스크가 없고, 다른 서버주소만 파라미터로 주어진 경우
				server_name = _msg.get_param(0);
				_msg.set_prefix(irc.find_member(socket->get_fd())->get_nick()); // 1. prefix에 nickname 추가
				if (server_name == irc.get_servername())
				{
					while (iter != irc.get_global_server().end())
					{
						server = (*iter).second;
						socket->write(Reply(RPL::LINKS(), "", server->get_name(), std::to_string(server->get_hopcount()), server->get_info()).get_msg().c_str());
						iter++;
					}
				}
				if (!(server = irc.get_server(server_name))) // 2. 해당 서버를 찾는다
					throw (ERR::NOSUCHSERVER(), _msg.get_param(0));
				server->get_socket()->write(_msg.get_msg()); // 3. 해당 서버쪽으로 메세지 전송
			}
			else
			{ // 현재 서버에서, 마스크와 매칭되는 모든 서버에 대한 정보를 전송
				mask = _msg.get_param(0);
				while (iter != irc.get_global_server().end())
				{
					server_name = (*iter).first;
					server = (*iter).second;
					if (ft::check_mask(server_name, mask)) // 마스크 일치
						socket->write(Reply(RPL::LINKS(), mask, server->get_name(), std::to_string(server->get_hopcount()), server->get_info()).get_msg().c_str());
					iter++;
				}
			}
		}
		else if (_msg.get_param_size() == 2) // 다른서버에 마스크와 함께 전송
		{
			_msg.set_prefix(irc.find_member(socket->get_fd())->get_nick()); // 1. prefix에 nickname 추가
			server_name = _msg.get_param(0);
			if (server_name == irc.get_servername()) // 만약에 내 주소라면
			{
				mask = _msg.get_param(1);
				while (iter != irc.get_global_server().end())
				{
					server_name = (*iter).first;
					server = (*iter).second;
					if (ft::check_mask(server_name, mask)) // 마스크 일치
						socket->write(Reply(RPL::LINKS(), mask, server->get_name(), std::to_string(server->get_hopcount()), server->get_info()).get_msg().c_str());
					iter++;
				}
				return ;
			}
			if (!(server = irc.get_server(_msg.get_param(0)))) // 2. 해당 서버를 찾는다
				throw (ERR::NOSUCHSERVER(), _msg.get_param(0));
			server->get_socket()->write(_msg.get_msg()); // 3. 해당 서버쪽으로 메세지 전송
		}
	}
	else if (socket->get_type() == SERVER)
	{
		server_name = _msg.get_param(0);
		if (server_name == irc.get_servername()) // 나에게 보낸거라면
		{
			Member	*member;
			std::string		nick_name;
			
			nick_name = _msg.get_prefix();
			member = irc.get_member(nick_name);
			std::map<std::string, Server *>::iterator	iter = irc.get_global_server().begin();
			if (_msg.get_param_size() == 1) // 마스크 없이 온 경우
			{
				while (iter != irc.get_global_server().end())
				{
					server = (*iter).second;
					member->get_socket()->write(Reply(RPL::LINKS(), "", server->get_name(), std::to_string(server->get_hopcount()), server->get_info()).get_msg().c_str());
					iter++;
				}
			}
			else if (_msg.get_param_size() == 2)
			{
				mask = _msg.get_param(1);
				while (iter != irc.get_global_server().end())
				{
					server = (*iter).second;
					if (ft::check_mask(server_name, mask)) // 마스크 일치
						member->get_socket()->write(Reply(RPL::LINKS(), mask, server->get_name(), std::to_string(server->get_hopcount()), server->get_info()).get_msg().c_str());
					iter++;
				}
			}
		}
		else // 내가 아니라면
		{
			server = irc.get_server(server_name);
			server->get_socket()->write(_msg.get_origin());
		}
	}
	else
	{
		return ;
	}
}

LinksCommand::LinksCommand(): Command()
{
}

LinksCommand::~LinksCommand()
{
}