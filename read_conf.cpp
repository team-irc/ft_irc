#include "read_conf.hpp"
#include "ft_irc.hpp"

ReadConf::ReadConf()
{
}

ReadConf::~ReadConf()
{
}

void    ReadConf::open_file()
{
    _fd = open(_file_name.c_str(), O_RDONLY);
}

void	read_config()
{
    char    *buf[BUFFER_SIZE];

    ft::read_until_crlf(_fd, buf, BUFFER_SIZE);
}