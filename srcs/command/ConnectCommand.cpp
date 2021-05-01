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
    if (socket->get_type() == UNKNOWN)
        throw (Reply(ERR::NOTREGISTERED()));
    else if (socket->get_type() == CLIENT)
    {
        if (member->check_mode('o', true))
            throw (Reply(ERR::NOPRIVILEGES()));
        if ((!remote.empty()) && !check_already_exist(irc, remote))
            throw (Reply(ERR::NOSUCHSERVER(), remote));
        if (remote.empty() || remote == irc.get_serverinfo().SERVER_NAME)
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
    else if (socket->get_type() == SERVER)
    {
        if (remote == irc.get_serverinfo().SERVER_NAME)
        {
            if (!check_already_exist(irc, target, port))
                connect_to_target(irc, target, port);
        }
        else
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
        }
    }
    if (param_size == 3)
    {
        if (ft::isdigit(_msg.get_param(1)))
            port    = _msg.get_param(1);
        else
        {
            port    = ft::itos(6667);
        }
        remote  = _msg.get_param(2);
    }
}

bool ConnectCommand::check_already_exist(IrcServer &irc, const std::string & target)
{
    std::map<std::string, Server *>::iterator first;
    std::map<std::string, Server *>::iterator last;

    first = irc.get_global_server().begin();
    last = irc.get_global_server().end();
    while (first != last)
    {
        const std::string current_servername(first->second->get_name());
        if (target == current_servername)
            return (true);
        ++first;
    }
    return (false);
}

// target = hostname, check target:port is already connect
bool ConnectCommand::check_already_exist(IrcServer &irc, const std::string & target, const std::string &port)
{
    std::map<std::string, Server *>::iterator first;
    std::map<std::string, Server *>::iterator last;

    first = irc.get_global_server().begin();
    last = irc.get_global_server().end();
    while (first != last)
    {
        const std::string current_hostname(first->second->get_socket()->get_hostname());
        const std::string current_port(ft::itos(first->second->get_socket()->get_port()));
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
        msg = target + ":" + port + ":" + irc.get_server(irc.get_serverinfo().SERVER_NAME)->get_password();
        if (ft::atoi(port.c_str()) % 2 == 0)
            new_socket = irc.get_ssl_listen_socket()->connect(msg.c_str(), irc.get_connect_ctx());
        else
            new_socket = irc.get_listen_socket()->connect(msg.c_str());
        new_socket->set_type(SERVER);
        tmp = irc.get_socket_set().add_socket(new_socket);
        if (irc.get_fdmax() < tmp)
            irc.get_fdmax() = tmp;
        msg = "PASS " + new_socket->get_pass() + "\n";
        new_socket->write(msg.c_str());
        irc.send_map_data(new_socket->get_fd());
        irc.send_user_data(new_socket->get_fd());
        irc.send_channel_data(new_socket->get_fd());
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