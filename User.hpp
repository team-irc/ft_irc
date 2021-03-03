#ifndef USER_HPP
# define USER_HPP

# include <string>

class User
{
private:
	std::string		_username;
	std::string		_nickname;

public:
	User();
	~User();
	User(User const &copy);
	User	&operator=(User const &ref);

	void	set_username(const std::string &username);
	void	set_nickname(const std::string &nickname);
	std::string		get_username();
	std::string		get_nickname();


};

#endif