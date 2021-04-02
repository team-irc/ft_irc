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

#define BUF_SIZE 512
#include "Socket.hpp"
#include "SocketSet.hpp"
#include "utils.hpp"
#include "Error.hpp"
#include "Message.hpp"
#include "CommandFactory.hpp"
#include "Member.hpp"
#include "Channel.hpp"
#include "Reply.hpp"

#define DEBUG 0

class IrcServer
{
private:
	std::string						_server_name;
	std::string						_my_pass;
	Socket							*_listen_socket;
	// std::vector<Socket *>		_socket_vector;
	SocketSet						_socket_set;
	int								_fd_max;
	Socket							*_current_sock;
	CommandFactory					_cmd_creator;

	// 연결된 서버/클라이언트에 접근하기 위한 fd 제공
	std::map<std::string, int>	_fd_map;
	// irc 네트워크상에 있는 유저의 정보 저장
	// unregistered 클라이언트용 map -> 처음 연결 시 -> 키 값을 port번호로 사용
	// 등록된 클라이언트용 map -> USER / NICK 입력 시, 추가되면 위쪽 MAP에서 제거, nick key값으로
	
	// std::map<std::string, Server *>		_global_server;
	std::map<std::string, Member *>		_global_user; // 전체 네트워크의 유저 닉네임, 전송하기 위한 fd 관리
	std::map<std::string, Channel *>	_global_channel;
	
	// std::map<std::string, struct>
	// idx  nickname	username	servername		| fd
	// 0	aaa			...			...				| 5
	// 1	bbb			...			...				| 5

public:
	IrcServer(int argc, char **argv);
	~IrcServer();

public:

	void				run(int argc);
	Socket				*get_current_socket();
	SocketSet			&get_socket_set();
	int					get_fdmax();
	void				send_msg(int send_fd, const char *msg);
	void				send_msg_server(int fd, const char *msg);
	void				add_channel(std::string &channel_name, Channel *channel);
	bool				add_member(std::string &nickname, Member *new_member);
	void				add_fd_map(const std::string &key, int fd);
	void				send_map_data(int my_fd);
	void				delete_member(const std::string &nickname);
	void				send_user_data(int fd);
	void				delete_fd_map(std::string const &key);
	Channel				*get_channel(std::string channel_name);
	Member				*get_member(std::string nick);
	Member				*get_member(int fd);
	std::map<std::string, Channel *>	&get_global_channel();
	std::map<std::string, Member *>		&get_global_user();
	Member				*find_member(int fd);
	bool				check_pass(Socket *currnet_socket);
	std::string			get_servername();
	std::map<std::string, int>			&get_fd_map();
	
private:

	void				echo_msg(int my_fd, const char *buf, int len);
	void				client_msg(int fd);
	void				unknown_msg(int fd);
	void				client_connect();

	void				connect_to_server(char **argv);
	void				send_msg(int my_fd, int except_fd, const char *msg);
	void				show_map_data();
	
	void				show_fd_map();
	void				show_global_user();
	void				show_global_channel();
	void				fd_event_loop();
};

void error_handling(const std::string buf);

#endif