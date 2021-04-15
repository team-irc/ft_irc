#include "Error.hpp"

Error::Error(std::string const &msg) : _msg(msg)
{
}

const char * Error::what() const throw()
{
	return (_msg.c_str());
}

Error::~Error() throw() {};