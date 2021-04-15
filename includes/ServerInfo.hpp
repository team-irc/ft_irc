#ifndef SERVER_INFO_HPP
# define SERVER_INFO_HPP

struct ServerInfo
{
	std::string SERVER_NAME;
	std::string	VERSION;
	std::string	OPERNAME;
	std::string	OPERPWD;
	std::string	ADMININFO1;
	std::string	ADMININFO2;
	std::string	ADMINEMAIL;
	std::string MOTD;
	int			PING_TIMEOUT;
	int			PONG_TIMEOUT;
};

#endif