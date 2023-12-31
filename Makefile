NAME = nibbler

SRCS :=	src/main.cpp \
		src/utils.cpp \
		src/Game.cpp \
		src/Menu/Menu.cpp \
		src/Menu/MenuItem.cpp \
		src/Menu/MenuCategory.cpp \
		src/LibHandler.cpp \
		src/Player.cpp \
		src/Food.cpp \
		src/ModesHandler.cpp \
		src/Score.cpp \
		src/Server.cpp \
		src/Client.cpp \

OBJS := $(patsubst %.cpp, %.o, $(SRCS))
DEPENDS := $(patsubst %.cpp, %.d, $(SRCS))

CXX			= 	g++
CXXFLAGS	=	-Wall -Wextra -std=c++20
RM			= 	rm -f

%.o : %.cpp Makefile
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@


all		:	${NAME}

${NAME}	:	${OBJS}
			${CXX} ${CXXFLAGS} $^ -o $@ -lncurses

-include $(DEPENDS)

#Should probably use a cmake..
#Download libs and make everything
install: 
			bash ./script.sh
			make -C . lib
			make -C .

#delete lib and cleans everything
uninstall: fclean
			rm -rf ./libs/raylib/raylib/

clean	:	
			make -C libs/sdl/ clean
			make -C libs/ncurses/ clean
			make -C libs/raylib/ clean
			make -C libs/raylib/sound/ clean
			${RM} ${OBJS} ${DEPENDS}

fclean	:	clean
			make -C libs/sdl/ fclean
			make -C libs/ncurses/ fclean
			make -C libs/raylib/ fclean
			make -C libs/raylib/sound/ fclean
			${RM} ${NAME}

re		:	fclean all lib

lib		: raylib
			make -C libs/sdl/
			make -C libs/ncurses/
			make -C libs/raylib/
			make -C libs/raylib/sound/

#make raylib.a
raylib:
			make -C  libs/raylib/raylib/src/

#clean raylib.a and objects
cleanraylib:
			make -C  libs/raylib/raylib/src/ clean

.PHONY : all clean fclean re lib raylib cleanraylib install uninstall
