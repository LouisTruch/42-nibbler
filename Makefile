MAKEFLAGS := -j9

NAME = nibbler
CXXFLAGS	:=	-Wall -Wextra -Werror -std=c++20 -DNDEBUG

ifeq ($(VALGRIND), 1)
	VALGRIND := valgrind --leak-check=full --show-leak-kinds=all
else
	VALGRIND :=
endif

ifeq ($(DEBUG), 1)
	CXXFLAGS := $(filter-out -DNDEBUG, $(CXXFLAGS))
	CXXFLAGS := $(filter-out -Werror, $(CXXFLAGS))
	CXXFLAGS += -DDEBUG -g
endif

SRCS := $(wildcard srcs/*.cpp)
SRCS += $(wildcard srcs/Menu/*.cpp)
SRCS += $(wildcard srcs/Log/*.cpp)
SRCS += $(wildcard srcs/Player/*.cpp)
SRCS += $(wildcard srcs/Mode/*.cpp)

OBJS := $(patsubst %.cpp, %.o, $(SRCS))
DEPENDS := $(patsubst %.cpp, %.d, $(SRCS))

CXX			= 	g++
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
			make -C libs/debug/ clean
			make -C libs/sdl/ clean
			make -C libs/ncurses/ clean
			make -C libs/raylib/ clean
			make -C libs/raylib/sound/ clean
			${RM} ${OBJS} ${DEPENDS}

fclean	:	clean
			make -C libs/debug/ fclean
			make -C libs/sdl/ fclean
			make -C libs/ncurses/ fclean
			make -C libs/raylib/ fclean
			make -C libs/raylib/sound/ fclean
			${RM} ${NAME}

re		:	fclean all lib

r 	:	all
			${VALGRIND} ./nibbler 10 10

lib		: raylib
			make -C libs/debug/
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
