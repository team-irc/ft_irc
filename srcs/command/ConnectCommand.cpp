#include "ConnectCommand.hpp"

// connect target <port> <remote>

void ConnectCommand::run(IrcServer &irc)
{
    Socket      *socket;
    Member      *member;
    std::string target;
    std::string port;
    std::string remote;

    parsing(target, port, remote);
    socket      = irc.get_current_socket();
    member      = irc.find_member(socket->get_fd());
    if ((!remote.empty()) && !check_already_exist(irc, remote, ft::itos(6667)))
        throw (Reply(ERR::NOSUCHSERVER(), remote));
    if (remote.empty() || remote == irc.get_listen_socket()->get_hostname())
    {
        if (!check_already_exist(irc, target, port))
            connect_to_target(irc, target, port);
    }
    else
    {
        _msg.set_prefix(member->get_nick());
        irc.send_msg(irc.find_server_fd(remote), _msg.get_msg());
    }
}

void ConnectCommand::parsing(std::string & target, std::string & port, std::string & remote)
{
    int param_size = _msg.get_param_size();

    if (param_size < 1)
        throw (Reply(ERR::NEEDMOREPARAMS(), "CONNECT"));
    target = _msg.get_param(0);
    if (param_size == 1)
    {
        port    = ft::itos(6667);
    }
    if (param_size == 2)
    {
        if (ft::isdigit(_msg.get_param(1)))
            port    = _msg.get_param(1);
        else
        {
            port    = ft::itos(6667);
            remote  = _msg.get_param(1);
        }
    }
    if (param_size == 3)
    {
        port    = _msg.get_param(1);
        remote  = _msg.get_param(2);
    }
}

bool ConnectCommand::check_already_exist(IrcServer &irc, const std::string & target, const std::string & port)
{
    std::map<std::string, Server *>::iterator first;
    std::map<std::string, Server *>::iterator last;

    first = irc.get_global_server().begin();
    last = irc.get_global_server().end();
    while (first != last)
    {
        const std::string current_port(ft::itos(first->second->get_socket()->get_port()));
        const std::string current_hostname(first->second->get_socket()->get_hostname());
        if (target == current_hostname && port == current_port)
            return (true);
        ++first;
    }
    return (false);
}

void ConnectCommand::connect_to_target(IrcServer & irc, const std::string & target, const std::string & port)
{
    Socket			*new_socket;
    std::string     msg;
	int				tmp;

    try
    {
        msg = target + ":" + port + ":asdf";
        new_socket = irc.get_listen_socket()->connect(msg.c_str());
        new_socket->set_type(SERVER);
        tmp = irc.get_socket_set().add_socket(new_socket);
        if (irc.get_fdmax() < tmp)
            irc.get_fdmax() = tmp;
        msg = "PASS " + new_socket->get_pass() + "\n";
        new_socket->write(msg.c_str());
        msg = "SERVER " + irc.get_serverinfo().SERVER_NAME + " 1 :" + irc.get_serverinfo().VERSION + "\n";
        irc.send_map_data(new_socket->get_fd());
        new_socket->write(msg.c_str());
        irc.send_user_data(new_socket->get_fd());
    }
    catch(Error & e)
    {
        irc.get_current_socket()->write(Reply(ERR::NOSUCHSERVER(), target));
    }
}

ConnectCommand::ConnectCommand(): Command()
{
}

ConnectCommand::~ConnectCommand()
{
}