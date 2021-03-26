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
	// _map.insert(std::pair<std::string, Command *>("PART", new PartCommand()));
	_map.insert(std::pair<std::string, Command *>("PRIVMSG", new PrivmsgCommand()));
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