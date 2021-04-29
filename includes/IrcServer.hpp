#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

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
#include "SSL_Socket.hpp"

#define DEBUG 0

class IrcServer
{
private:
	std::string						_my_pass;
	Socket							*_listen_socket;
	SSL_Socket						*_ssl_listen_socket;

	SocketSet						_socket_set;
	int								_fd_max;
	Socket							*_current_sock;
	CommandFactory					_cmd_creator;

	// SSL
	SSL_CTX							*_accept_ctx;
	SSL_CTX							*_connect_ctx;

	time_t							_start_time;
	time_t							_current_time;

	// irc 네트워크상에 있는 유저의 정보 저장
	// unregistered 클라이언트용 map -> 처음 연결 시 -> 키 값을 port번호로 사용
	// 등록된 클라이언트용 map -> USER / NICK 입력 시, 추가되면 위쪽 MAP에서 제거, nick key값으로
	
	std::map<std::string, Server *>		_global_server;
	std::map<std::string, Member *>		_global_user; // 전체 네트워크의 유저 닉네임, 전송하기 위한 fd 관리
	std::map<std::string, Channel *>	_global_channel;
	std::vector<Member>					_user_history;
	// _user_history.insert(*member)

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
	void						set_current_socket_null();
	Socket						*get_listen_socket();
	SSL_Socket					*get_ssl_listen_socket();
	SSL_CTX						*get_accept_ctx();
	SSL_CTX						*get_connect_ctx();
	SocketSet					&get_socket_set();
	int							&get_fdmax();
	void						send_msg(int send_fd, const char *msg);
	void						send_msg_server(int fd, const char *msg);
	
	void						add_channel(std::string const &channel_name, Channel *channel);
	void						delete_channel(const std::string &channel_name);
	Channel						*get_channel(std::string channel_name);

	
	bool						add_member(std::string &nickname, Member *new_member);
	void						delete_member(const std::string &nickname);
	Member						*find_member(int fd);

	bool						add_server(const std::string &server_name, const std::string &hopcount, int token, const std::string &info, Socket *socket);
	void						delete_server(const std::string &server_name);
	Server						*get_server(const std::string &server_name);
	int							find_server_fd(const std::string &server_name);

	void						send_map_data(int my_fd);
	void						send_user_data(int fd);
	void						send_channel_data(int fd);
	Member						*get_member(std::string nick);
	Member						*get_member(int fd);
	std::map<std::string, Channel *>	&get_global_channel();
	std::map<std::string, Member *>		&get_global_user();
	std::map<std::string, Server *>		&get_global_server();
	std::vector<Member>					&get_user_history();
	bool				check_pass(Socket *currnet_socket);
	struct ServerInfo	&get_serverinfo();
	time_t				get_start_time();
  
	
	std::string			get_version();
	std::string			get_debug_level();

	int					get_server_token();

	bool				check_oper(const std::string & id, const std::string & pwd);
	void				print_serverinfo();
	void				print_motd();
	CommandFactory		&get_command_factory();
	
private:

	void				echo_msg(int my_fd, const char *buf, int len);
	int					client_msg(int fd);
	void				unknown_msg(int fd);
	void				client_connect();
	void				ssl_connect();

	void				connect_to_server(char **argv);
	void				send_msg(int my_fd, int except_fd, const char *msg);
	void				show_map_data();
	
	void				show_global_server();
	void				show_global_user();
	void				show_global_channel();
	void				fd_event_loop();

	void				update_last_time();
	void				check_connection();
	bool				is_reply_code(std::string const &command);

	void				init_ssl_setting();
};

void error_handling(const std::string buf);

#endif