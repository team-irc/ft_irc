.PHONY: all clean fclean re

NAME = server

CC = clang++

# CFLAGS = -Wall -Werror -Wextra -std=c++98 -g

CFLAGS = -std=c++98

HEADER_DIR = . 

SRC_DIR = .

SRC = main.cpp	ft_irc.cpp	Socket.cpp	SocketSet.cpp	utils.cpp	\
		Error.cpp	Message.cpp\
		Command.cpp		CommandFactory.cpp		command/ServerCommand.cpp

SRCS = $(addprefix $(SRC_DIR)/, $(SRC))

# OBJS = $(SRCS:%.cpp=%.o)

## 나중에 $(CLFAG) 추가
$(NAME) : $(SRCS)
	$(CC) $(SRCS) $(CFLAGS) -I $(HEADER_DIR) -I ./command -o $(NAME) -g

# %.o : %.c
# 		$(CC) -o $@ -c $^ -I $(HEADER_DIR) -I ./command

all : $(NAME)

# clean :
# 		rm -f $(OBJS)

fclean :
	rm -f $(NAME)

re : fclean all