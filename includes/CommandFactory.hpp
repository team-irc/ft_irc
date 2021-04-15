/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandFactory.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hna <hna@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 22:33:51 by marvin            #+#    #+#             */
/*   Updated: 2021/03/11 00:01:09 by hna              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDFACTORY_HPP
# define COMMANDFACTORY_HPP

# include <iostream>
# include <map>

class Command;

class CommandFactory
{
	public:
		CommandFactory();
		CommandFactory(const CommandFactory &ref);
		CommandFactory &operator=(const CommandFactory &ref);
		// 서버/클라이언트가 필요한 명령어를 요청 시 반환(요청은 std::String 형식, 반환은 Command *)
		Command	*get_command(const std::string &cmd);
		~CommandFactory();
	private:
		std::map<std::string, Command *>	_map;
};

# include "Command.hpp"

#endif