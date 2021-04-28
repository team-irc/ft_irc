#include "read_conf.hpp"

ReadConf::ReadConf()
{
}

ReadConf::~ReadConf()
{
    close(_fd);
}

void        ReadConf::open_file(const std::string & file_name)
{
    if ((_fd = open(file_name.c_str(), O_RDONLY)) < 0)
        throw (Error("Cannot open CONF: " + file_name));
}

void	    ReadConf::read_config(ServerInfo & si)
{
    char    buf[BUFFER_SIZE];
    int     str_len;
    int     line_number;

    line_number = 1;
    while (1)
    {
        std::string *split_ret;
        int         result;

        result = ft::read_until_crlf(_fd, buf, &str_len);
        ft::split(std::string(buf), '=', split_ret);
        ft::rtrim(split_ret[0], ' ');
        ft::ltrim(split_ret[1], ' ');
        ft::rtrim(split_ret[1], '\n');
        set_si(si, split_ret[0], split_ret[1], line_number);
        delete[] split_ret;
        line_number += 1;
        if (result <= 0)
            break ;
    }
}

void	    ReadConf::set_si(ServerInfo & si, const std::string & key, const std::string & value, int line_num)
{
    if (key.empty() || value.empty())
        throw (Error("CONFIG ERROR in line number " + ft::itos(line_num)));
    if (key == "SERVER_NAME")
        si.SERVER_NAME = value;
    else if (key == "VERSION")
        si.VERSION = value;
    else if (key == "ADMININFO1")
        si.ADMININFO1 = value;
    else if (key == "ADMININFO2")
        si.ADMININFO2 = value;
    else if (key == "ADMINEMAIL")
        si.ADMINEMAIL = value;
    else if (key == "OPERNAME")
        si.OPERNAME = value;
    else if (key == "OPERPWD")
        si.OPERPWD = value;
    else if (key == "MOTD")
        si.MOTD = read_motd(value);
    else if (key == "PING_TIMEOUT")
        si.PING_TIMEOUT = ft::atoi(value.c_str());
    else if (key == "PONG_TIMEOUT")
        si.PONG_TIMEOUT = ft::atoi(value.c_str());
    else
        throw (Error("CONFIG ERROR in line number " + ft::itos(line_num)));
}

std::string	ReadConf::read_motd(const std::string & file_name)
{
    int         str_len;
    int         motd_fd;
    char        buf[BUFFER_SIZE];
    std::string ret;

    if ((motd_fd = open(file_name.c_str(), O_RDONLY)) < 0)
        throw (Error("Cannot open MOTD: " + file_name));
    while (1)
    {
        int result = ft::read_until_crlf(motd_fd, buf, &str_len);
        ret += buf;
        if (result <= 0)
            break ;
    }
    close(motd_fd);
    return (ret);
}