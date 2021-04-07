#ifndef FT_IRC_HPP
# define FT_IRC_HPP

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <map>
#include <ctime>

#include "Socket.hpp"
#include "SocketSet.hpp"
#include "utils.hpp"
#include "Error.hpp"
#include "Message.hpp"
#include "CommandFactory.hpp"
#include "Member.hpp"
#include "Channel.hpp"
#include "Reply.hpp"
#include "ServerInfo.hpp"
#include "read_conf.hpp"
#include "Server.hpp"

#define DEBUG 0

class IrcServer
{
private:
	std::string						_my_pass;
	Socket							*_listen_socket;
	// std::vector<Socket *>		_socket_vector;
	SocketSet						_socket_set;
	int								_fd_max;
	Socket							*_current_sock;
	CommandFactory					_cmd_creator;

	time_t							_start_time;

	// 연결된 서버/클라이언트에 접근하기 위한 fd 제공
	std::map<std::string, int>		_fd_map;
	// irc 네트워크상에 있는 유저의 정보 저장
	// unregistered 클라이언트용 map -> 처음 연결 시 -> 키 값을 port번호로 사용
	// 등록된 클라이언트용 map -> USER / NICK 입력 시, 추가되면 위쪽 MAP에서 제거, nick key값으로
	
	std::map<std::string, Server *>		_global_server;
	std::map<std::string, Member *>		_global_user; // 전체 네트워크의 유저 닉네임, 전송하기 위한 fd 관리
	std::map<std::string, Channel *>	_global_channel;

	struct ServerInfo					_si;
	// std::map<std::string, struct>
	// idx  nickname	username	servername		| fd
	// 0	aaa			...			...				| 5
	// 1	bbb			...			...				| 5

public:
	IrcServer(int argc, char **argv);
	~IrcServer();

public:

	void						run(int argc);
	Socket						*get_current_socket();
	SocketSet					&get_socket_set();
	int							get_fdmax();
	void						send_msg(int send_fd, const char *msg);
	void						send_msg_server(int fd, const char *msg);
	
	void						add_channel(std::string &channel_name, Channel *channel);
	Channel						*get_channel(std::string channel_name);

	
	bool						add_member(std::string &nickname, Member *new_member);
	void						delete_member(const std::string &nickname);
	Member						*find_member(int fd);

	bool						add_server(const std::string &server_name, const std::string &hopcount, const std::string &info, Socket *socket);
	void						delete_server(const std::string &server_name);
	Server						*get_server(const std::string &server_name);
	
	void						add_fd_map(const std::string &key, int fd);
	void						delete_fd_map(std::string const &key);
	std::map<std::string, int>	&get_fd_map();
	int							find_fd_map(std::string const &server_name);

	void						send_map_data(int my_fd);
	void						send_user_data(int fd);
	Member						*get_member(std::string nick);
	Member						*get_member(int fd);
	std::map<std::string, Channel *>	&get_global_channel();
	std::map<std::string, Member *>		&get_global_user();
	std::map<std::string, Server *>		&get_global_server();
	bool				check_pass(Socket *currnet_socket);
	struct ServerInfo	&get_serverinfo();
	time_t				get_start_time();
  
	
	std::string			get_version();
	std::string			get_debug_level();
	bool				check_oper(const std::string & id, const std::string & pwd);

	void				run_command(const std::string &msg);
	
private:

	void				echo_msg(int my_fd, const char *buf, int len);
	void				client_msg(int fd);
	void				unknown_msg(int fd);
	void				client_connect();

	void				connect_to_server(char **argv);
	void				send_msg(int my_fd, int except_fd, const char *msg);
	void				show_map_data();
	
	void				show_fd_map();
	void				show_global_server();
	void				show_global_user();
	void				show_global_channel();
	void				fd_event_loop();

	bool				is_reply_code(std::string const &command);
};

void error_handling(const std::string buf);

#endif