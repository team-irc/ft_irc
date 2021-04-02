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
#include "IsonCommand.hpp"
#include "OperCommand.hpp"

CommandFactory::CommandFactory()
{
	// 명령어 만들면 여기다가 추가
	_map.insert(std::pair<std::string, Command *>("SERVER", new ServerCommand()));
	_map.insert(std::pair<std::string, Command *>("PASS", new PassCommand()));
	_map.insert(std::pair<std::string, Command *>("USER", new UserCommand()));
	_map.insert(std::pair<std::string, Command *>("NICK", new NickCommand()));
	_map.insert(std::pair<std::string, Command *>("JOIN", new JoinCommand()));
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
	_map.insert(std::pair<std::string, Command *>("ISON", new IsonCommand()));
	_map.insert(std::pair<std::string, Command *>("OPER", new OperCommand()));
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

CommandFactory::~CommandFactory()
{
	_map.clear();
}