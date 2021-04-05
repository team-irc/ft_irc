#include "InfoCommand.hpp"

/*
      Command: INFO
   Parameters: [<server>]

   The INFO command is required to return information which describes
   the server: its version, when it was compiled, the patchlevel, when
   it was started, and any other miscellaneous information which may be
   considered to be relevant.

   Numeric Replies:

           ERR_NOSUCHSERVER
           RPL_INFO                        RPL_ENDOFINFO

   Examples:

   INFO csd.bu.edu                 ; request an INFO reply from
   csd.bu.edu

   :Avalon INFO *.fi               ; INFO request from Avalon for first
                                   server found to match *.fi.

   INFO Angel                      ; request info from the server that
                                   Angel is connected to.
*/

void InfoCommand::run(IrcServer &irc)
{
    Socket      *socket;
    Member      *member;
    std::string ret;

    socket = irc.get_current_socket();
    member = irc.find_member(socket->get_fd());
    ret = irc.get_servername() + ", VERSION: " + SERVER_CONST::VERSION;
    if (socket->get_type() == CLIENT)
    {
        if (_msg.get_param_size() == 0)
        {
            socket->write(Reply(RPL::INFO(), ret).get_msg().c_str());
            socket->write(Reply(RPL::ENDOFINFO()).get_msg().c_str());
        }
        else // 다른 서버의 인포를 찾는경우
        {
            int target_fd;
            Socket  *next_server;

            _msg.set_prefix(member->get_nick());
            target_fd = irc.find_fd_map(_msg.get_param(0));
            if (target_fd == 0)
                throw (Reply(ERR::NOSUCHSERVER(), _msg.get_param(0)));
            next_server = irc.get_socket_set().find_socket(target_fd);
            next_server->write(_msg.get_msg());
        }
    }
    else if (socket->get_type() == SERVER)
    {
        if (irc.get_servername() == _msg.get_param(0))
        {
            Reply   rpl;
            rpl.set_username(_msg.get_prefix());
            rpl.set_servername(_msg.get_param(0));
            socket->write(Reply(RPL::INFO(), ret).get_msg().c_str());
            socket->write(Reply(RPL::ENDOFINFO()).get_msg().c_str());
        }
    }
}

InfoCommand::InfoCommand(): Command()
{
}