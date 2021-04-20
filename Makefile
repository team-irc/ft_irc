.PHONY: all clean fclean re

_END		=	\033[0;0m
_RED		=	\033[0;31m
_GREEN		=	\033[0;32m

NAME = ircserv

CXX = clang++

CXXFLAGS = -std=c++98 -I ${INC_DIR} -I ${INC_DIR2} -g -fsanitize=address -I ${SSL_DIR}

SSL_DIR = ./ssl
INC_DIR = ./includes
INC_DIR2 = ./includes/command

SRCS = $(wildcard ./srcs/*.cpp)
SRCS += $(wildcard ./srcs/command/*.cpp)

OBJS = $(patsubst %.cpp, %.o, ${SRCS})

LIBS = $(wildcard ./ssl/*.a)

$(NAME) : $(OBJS)
	@$(CXX) $(OBJS) $(CXXFLAGS) $(LIBS) -o $(NAME) 
	@echo "${_GREEN}COMPILE COMPLETE${_END}"

%.o : %.cpp
	@echo "${_GREEN}COMPILING...${_END}"
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
