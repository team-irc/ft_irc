#ifndef READ_CONF_HPP
# define READ_CONF_HPP

# include "Error.hpp"
# include "utils.hpp"
# include "ServerInfo.hpp"
# include <fcntl.h>

class ReadConf
{
private:
	int	_fd;
public:
	ReadConf();
	~ReadConf();
public:
	void		open_file(const std::string & file_name);
	void		read_config(ServerInfo & si);
private:
	std::string	read_motd(const std::string & file_name);
	void		set_si(ServerInfo & si, const std::string & key, const std::string & value, int line_num);
};

#endif