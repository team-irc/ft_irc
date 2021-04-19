.PHONY: all clean fclean re

_END		=	\033[0;0m
_RED		=	\033[0;31m
_GREEN		=	\033[0;32m

NAME = server

CXX = clang++

<<<<<<< HEAD
CXXFLAGS = -std=c++98 -I ${INC_DIR} -I ${INC_DIR2} -g -fsanitize=address
=======
CXXFLAGS = -std=c++98 -I ${INC_DIR} -I ${INC_DIR2} -g -fsanitize=address -I ${SSL_DIR} -L ./${SSL_DIR} -lssl -lcrypto
>>>>>>> a337ad65caccafeb85fe758534a99dd2af8c1923

SSL_DIR = ./ssl
INC_DIR = ./includes
INC_DIR2 = ./includes/command

SRCS = $(wildcard ./srcs/*.cpp)
SRCS += $(wildcard ./srcs/command/*.cpp)

OBJS = $(patsubst %.cpp, %.o, ${SRCS})

$(NAME) : $(OBJS)
	@$(CXX) $(OBJS) $(CXXFLAGS) -o $(NAME)
	@echo "${_GREEN}COMPILE COMPLETE${_END}"

%.o : %.cpp
	@$(CXX) -o $@ -c $^ $(CXXFLAGS)

all : $(NAME)

clean :
	@echo "${_RED}REMOVE OBJECTS...${_END}"
	@rm -f $(OBJS)

fclean : clean
	@echo "${_RED}REMOVE PROGRAM...${_END}"
	@rm -f $(NAME)
	@rm -rf server.dSYM

re : fclean all