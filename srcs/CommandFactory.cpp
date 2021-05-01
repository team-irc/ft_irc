#include "CommandFactory.hpp"
#include "ServerCommand.hpp"
#include "NickCommand.hpp"
#include "UserCommand.hpp"
#include "PassCommand.hpp"
#include "JoinCommand.hpp"
#include "PartCommand.hpp"
#include "PrivmsgCommand.hpp"
#include "QuitCommand.hpp"
#include "NamesCommand.hpp"
#include "SquitCommand.hpp"
#include "TopicCommand.hpp"
#include "ModeCommand.hpp"
#include "ListCommand.hpp"
#include "AwayCommand.hpp"
#include "KickCommand.hpp"
#include "InviteCommand.hpp"
#include "VersionCommand.hpp"
#include "IsonCommand.hpp"
#include "OperCommand.hpp"
#include "UserhostCommand.hpp"
#include "TimeCommand.hpp"
#include "InfoCommand.hpp"
#include "LinksCommand.hpp"
#include "NoticeCommand.hpp"
#include "AdminCommand.hpp"
#include "RehashCommand.hpp"
#include "PingCommand.hpp"
#include "PongCommand.hpp"
#include "TraceCommand.hpp"
#include "WhoQuery.hpp"
#include "WhoisQuery.hpp"
#include "WhowasQuery.hpp"
#include "ConnectCommand.hpp"
#include "StatsCommand.hpp"
#include "ErrorCommand.hpp"
#include "KillCommand.hpp"
#include "NjoinCommand.hpp"

CommandFactory::CommandFactory()
{
	// 명령어 만들면 여기다가 추가
	_map.insert(std::pair<std::string, Command *>("SERVER", new ServerCommand()));
	_map.insert(std::pair<std::string, Command *>("PASS", new PassCommand()));
	_map.insert(std::pair<std::string, Command *>("USER", new UserCommand()));
	_map.insert(std::pair<std::string, Command *>("NICK", new NickCommand()));
	_map.insert(std::pair<std::string, Command *>("JOIN", new JoinCommand()));
	_map.insert(std::pair<std::string, Command *>("NJOIN", new NjoinCommand()));
	_map.insert(std::pair<std::string, Command *>("QUIT", new QuitCommand()));
	_map.insert(std::pair<std::string, Command *>("NAMES", new NamesCommand()));
	_map.insert(std::pair<std::string, Command *>("PART", new PartCommand()));
	_map.insert(std::pair<std::string, Command *>("PRIVMSG", new PrivmsgCommand()));
	_map.insert(std::pair<std::string, Command *>("SQUIT", new SquitCommand()));
	_map.insert(std::pair<std::string, Command *>("TOPIC", new TopicCommand()));
	_map.insert(std::pair<std::string, Command *>("MODE", new ModeCommand()));
	_map.insert(std::pair<std::string, Command *>("LIST", new ListCommand()));
	_map.insert(std::pair<std::string, Command *>("AWAY", new AwayCommand()));
	_map.insert(std::pair<std::string, Command *>("KICK", new KickCommand()));
	_map.insert(std::pair<std::string, Command *>("INVITE", new InviteCommand()));
	_map.insert(std::pair<std::string, Command *>("VERSION", new VersionCommand()));
	_map.insert(std::pair<std::string, Command *>("ISON", new IsonCommand()));
	_map.insert(std::pair<std::string, Command *>("OPER", new OperCommand()));
	_map.insert(std::pair<std::string, Command *>("USERHOST", new UserhostCommand()));
	_map.insert(std::pair<std::string, Command *>("TIME", new TimeCommand()));
	_map.insert(std::pair<std::string, Command *>("INFO", new InfoCommand()));
	_map.insert(std::pair<std::string, Command *>("LINKS", new LinksCommand()));
	_map.insert(std::pair<std::string, Command *>("NOTICE", new NoticeCommand()));
	_map.insert(std::pair<std::string, Command *>("ADMIN", new AdminCommand()));
	_map.insert(std::pair<std::string, Command *>("REHASH", new RehashCommand()));
	_map.insert(std::pair<std::string, Command *>("PING", new PingCommand()));
	_map.insert(std::pair<std::string, Command *>("PONG", new PongCommand()));
	_map.insert(std::pair<std::string, Command *>("TRACE", new TraceCommand()));
	_map.insert(std::pair<std::string, Command *>("WHO", new WhoQuery()));
	_map.insert(std::pair<std::string, Command *>("WHOIS", new WhoisQuery()));
	_map.insert(std::pair<std::string, Command *>("WHOWAS", new WhowasQuery()));
	_map.insert(std::pair<std::string, Command *>("CONNECT", new ConnectCommand()));
	_map.insert(std::pair<std::string, Command *>("STATS", new StatsCommand()));
	_map.insert(std::pair<std::string, Command *>("ERROR", new ErrorCommand()));
	_map.insert(std::pair<std::string, Command *>("KILL", new KillCommand()));
}

CommandFactory::CommandFactory(const CommandFactory &ref) : _map(ref._map)
{}

CommandFactory &CommandFactory::operator=(const CommandFactory &ref)
{
	if (this == &ref) return (*this);
	this->_map = ref._map;
	return (*this);
}

Command	*CommandFactory::get_command(const std::string &cmd)
{
	std::map<std::string, Command *>::iterator find = _map.find(cmd);
	if (find == _map.end())
		return (NULL);
	else
		return (find->second);
}

std::map<std::string, Command *>	&CommandFactory::get_command_map()
{
	return (_map);
}

CommandFactory::~CommandFactory()
{
	_map.clear();
}