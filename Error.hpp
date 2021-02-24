#ifndef ERROR_HPP
# define ERROR_HPP

# include <exception>
# include <string>

class Error : std::exception
{
private:
	std::string		_msg;
public:
	virtual ~Error() throw() {};
	Error(std::string const &msg);
	virtual const char * what() const throw();
};

Error::Error(std::string const &msg) : _msg(msg)
{
}

const char * Error::what() const throw()
{
	return (_msg.c_str());
}

#endif