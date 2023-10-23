NAME = nibbler

SRCS =	src/main.cpp \
		src/Game.cpp \
		src/LibHandler.cpp \
		src/Player.cpp \

OBJS	=	${SRCS:.cpp=.o}

CXX			= 	g++
CXXFLAGS	=	-Wall -Wextra  -std=c++20 -g
RM			= 	rm -f

.cpp.o	:	
			${CXX} ${CXXFLAGS} -c $< -o ${<:.cpp=.o}

all		:	${NAME}

${NAME}	:	${OBJS}
			make -C libs/sdl/
			${CXX} ${CXXFLAGS} -o ${NAME} ${OBJS}

clean	:	
			make -C libs/sdl/ clean
			${RM} ${OBJS}

fclean	:	clean
			make -C libs/sdl/ fclean
			${RM} ${NAME}

re		:	fclean all

lib		:
			make -C libs/sdl/

.PHONY : all clean fclean re
