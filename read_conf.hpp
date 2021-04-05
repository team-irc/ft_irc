#ifndef READ_CONF_HPP
# define READ_CONF_HPP

# include "utils.hpp"
# include <fcntl.h>

class ReadConf
{
private:
	int	_fd;
public:
	ReadConf();
	~ReadConf();
public:
	void	open_file(const std::string & file_name);
	void	read_config();
};

#endif