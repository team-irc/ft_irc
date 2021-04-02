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

void LinksCommand::run(IrcServer &irc)
{
	Socket			*socket;

	socket = irc.get_current_socket();
	if (socket->get_type() == CLIENT)
	{
		
	}
	else if (socket->get_type() == SERVER)
	{
		
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