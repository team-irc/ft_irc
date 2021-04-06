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
    int     line_number;

    line_number = 1;
    while (ft::read_until_crlf(_fd, buf, &str_len))
    {
        std::string tmp(buf);
        std::string *split_ret;

        if (ft::split(tmp, '=', split_ret) != 2)
            throw (Error("CONFIG ERROR in line number " + std::to_string(line_number)));
        ft::rtrim(split_ret[0], ' ');
        ft::ltrim(split_ret[1], ' ');
        ft::rtrim(split_ret[1], '\n');
        std::string key = split_ret[0];
        std::string value = split_ret[1];
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
        else
            throw (Error("CONFIG ERROR in line number " + std::to_string(line_number)));
        delete[] split_ret;
        line_number += 1;
    }
}