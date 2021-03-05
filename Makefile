.PHONY: all clean fclean re

NAME = server

CC = clang++

CFLAGS = -Wall -Werror -Wextra -std=c++98 -fsanitize=address -g

HEADER_DIR = . 

SRC_DIR = .

SRC = main.cpp	ft_irc.cpp	Socket.cpp	SocketSet.cpp	utils.cpp	Error.cpp	Message.cpp	\
		Command.cpp		CommandFactory.cpp		command/SERVER.cpp

SRCS = $(addprefix $(SRC_DIR)/, $(SRC))

# OBJS = $(SRCS:%.cpp=%.o)

## 나중에 $(CLFAG) 추가
$(NAME) : $(SRCS)
	$(CC) $(SRCS) -I $(HEADER_DIR) -I ./command -o $(NAME)

# %.o : %.c
# 		$(CC) -o $@ -c $^ -I $(HEADER_DIR) -I ./command

all : $(NAME)

# clean :
# 		rm -f $(OBJS)

fclean :
	rm -f $(NAME)

re : fclean all