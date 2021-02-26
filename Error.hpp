#ifndef ERROR_HPP
# define ERROR_HPP

# include <exception>
# include <string>

class Error : std::exception
{
private:
	std::string		_msg;
public:
	Error(std::string const &msg);
	virtual ~Error() throw();
	virtual const char * what() const throw();
};

#endif