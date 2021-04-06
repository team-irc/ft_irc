#include "read_conf.hpp"

ReadConf::ReadConf()
{
}

ReadConf::~ReadConf()
{
}

void    ReadConf::open_file(const std::string & file_name)
{
    _fd = open(file_name.c_str(), O_RDONLY);
}

void	ReadConf::read_config(ServerInfo & si)
{
    char    buf[BUFFER_SIZE];
    int     str_len;

    while (ft::read_until_crlf(_fd, buf, &str_len))
    {
        std::string tmp(buf);
        std::string *split_ret;

        if (ft::split(tmp, '=', split_ret) != 2)
            std::cout << "CONFIGFILE ERROR" << std::endl;
        ft::rtrim(split_ret[0], ' ');
        ft::ltrim(split_ret[1], ' ');
        if (split_ret[0] == "Name")
            si.NAME = split_ret[1];
        if (split_ret[0] == "AdminInfo1")
            si.ADMININFO1 = split_ret[1];
        if (split_ret[0] == "AdminInfo2")
            si.ADMININFO2 = split_ret[1];
        if (split_ret[0] == "AdminEMail")
            si.ADMINEMAIL = split_ret[1];
        else
            std::cout << "CONFIGFILE ERROR" << std::endl;
        delete[] split_ret;
    }
}