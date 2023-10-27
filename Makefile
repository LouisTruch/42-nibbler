NAME = nibbler

SRCS =	src/main.cpp \
		src/Game.cpp \
		src/LibHandler.cpp \
		src/Player.cpp \
		src/Food.cpp \

OBJS	=	${SRCS:.cpp=.o}

CXX			= 	g++
CXXFLAGS	=	-Wall -Wextra  -std=c++20 -g
RM			= 	rm -f

.cpp.o	:	
			${CXX} ${CXXFLAGS} -c $< -o ${<:.cpp=.o}

all		:	${NAME}

${NAME}	:	${OBJS}
			${CXX} ${CXXFLAGS} -o ${NAME} ${OBJS}

clean	:	
			make -C libs/sdl/ clean
			make -C libs/ncurses/ clean
			make -C libs/sfml/ clean
			${RM} ${OBJS}

fclean	:	clean
			make -C libs/sdl/ fclean
			make -C libs/ncurses/ fclean
			make -C libs/sfml/ clean
			${RM} ${NAME}

re		:	fclean all lib

lib		:
			make -C libs/sdl/
			make -C libs/ncurses/
			make -C libs/sfml/

mlxinstall:
		bash ./script.sh

mlxclean:
		rm -rf libs/mlx/lib/build/*

mlxdelete:
		rm -rf libs/mlx/lib/

.PHONY : all clean fclean re
