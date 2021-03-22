#ifndef REPLY_HPP
# define REPLY_HPP

# include <iostream>

// prefix replies(numeric) nick [error] <msg>
// error는 해당 에러를 발생하는 문자열(ex. No such nick or channel의 경우엔 nick이나 channel의 이름)
// msg는 명령어 마다 다름, Command에서 처리해서 넘겨줌

class Reply {
	private:
		std::string _msg;
		std::string replies;
		std::string nick;
		std::vector<std::string> param;
	public:
		Reply();
		Reply(const Reply &ref);
		Reply &operator=(const Reply &ref);
		const std::string &get_msg();
};