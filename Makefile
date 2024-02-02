MAKEFLAGS := -j9

NAME = nibbler
CXXFLAGS	:=	-Wall -Wextra -Werror -std=c++20 -DNDEBUG
LDFLAGS		:=	-lncurses

ENV :=

ifeq ($(VALGRIND), 1)
	VALGRIND := valgrind --leak-check=full --show-leak-kinds=all
else
	VALGRIND :=
endif

ifeq ($(DEBUG), 1)
	CXXFLAGS := $(filter-out -DNDEBUG, $(CXXFLAGS))
	CXXFLAGS := $(filter-out -Werror, $(CXXFLAGS))
	CXXFLAGS += -DDEBUG -g
	ENV += DEBUG=1
else
	ENV += DEBUG=0
endif

ifeq ($(MANUAL_INSTALLSDL), 1)
	ENV += MANUAL_INSTALLSDL=1
endif

SRCS := $(wildcard srcs/*.cpp)
SRCS += $(wildcard srcs/Menu/*.cpp)
SRCS += $(wildcard srcs/Log/*.cpp)
SRCS += $(wildcard srcs/Mode/*.cpp)
SRCS += $(wildcard srcs/Game/*.cpp)
SRCS += $(wildcard srcs/Game/Player/*.cpp)
SRCS += $(wildcard srcs/Network/*.cpp)

OBJS := $(patsubst %.cpp, %.o, $(SRCS))
DEPENDS := $(patsubst %.cpp, %.d, $(SRCS))

CXX			= 	g++
RM			= 	rm -f

%.o : %.cpp Makefile
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

green = /bin/echo -e "\x1B[32m$1\x1b[0m"
all		:  +subtargets ${NAME}
			@$(call green,"Successfully compiled Nibbler.")

${NAME}	:	${OBJS}
			${CXX} ${CXXFLAGS} $^ -o $@ $(LDFLAGS)


-include $(DEPENDS)

clean	:	
			$(MAKE) -C libs/debug/ clean
			$(MAKE) -C libs/debug/sound clean
			$(MAKE) -C libs/sdl/ clean
			$(MAKE) -C libs/ncurses/ clean
			$(MAKE) -C libs/raylib/ clean
			$(MAKE) -C libs/raylib/sound/ clean
			${RM} ${OBJS} ${DEPENDS}

fclean	:	clean
			$(MAKE) -C libs/debug/ fclean
			$(MAKE) -C libs/debug/sound/ fclean
			$(MAKE) -C libs/sdl/ fclean
			$(MAKE) -C libs/ncurses/ fclean
			$(MAKE) -C libs/raylib/ fclean
			$(MAKE) -C libs/raylib/sound/ fclean
			${RM} ${NAME}

+re		:	fclean all

r 	:	all
			${VALGRIND} ./nibbler 10 10

+subtargets: 
			@$(call green,"Compiling subtargets")
			$(ENV) $(MAKE) -C libs/debug
			$(ENV) $(MAKE) -C libs/debug/sound
			$(ENV) $(MAKE) -C libs/sdl
			$(ENV) $(MAKE) -C libs/ncurses
			$(ENV) $(MAKE) -C libs/raylib
			$(ENV) $(MAKE) -C libs/raylib/sound

cleanlibs: cleanraylib cleansdl

cleanraylib:
			rm -rf ./libs/raylib/raylib

cleansdl:
			rm -rf ./libs/sdl/SDL


.PHONY : all clean fclean re lib raylib cleanraylib install uninstall
