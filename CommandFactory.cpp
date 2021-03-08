#include "CommandFactory.hpp"
#include "ServerCommand.hpp"

CommandFactory::CommandFactory()
{
	_map.insert(std::pair<std::string, Command *>("SERVER", new ServerCommand()));
	// 명령어 만들면 여기다가 추가
}

CommandFactory::CommandFactory(const CommandFactory &ref) : _map(ref._map)
{}

CommandFactory &CommandFactory::operator=(const CommandFactory &ref)
{
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