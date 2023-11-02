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

OBJS := $(patsubst %.cpp, %.o, $(SRCS))
DEPENDS := $(patsubst %.cpp, %.d, $(SRCS))

CXX			= 	g++
CXXFLAGS	=	-Wall -Wextra  -std=c++20 -g
RM			= 	rm -f

%.o : %.cpp Makefile
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# .cpp.o	:	
# 			${CXX} ${CXXFLAGS} -c $< -o ${<:.cpp=.o}

all		:	${NAME}

${NAME}	:	${OBJS}
			${CXX} ${CXXFLAGS} $^ -o $@ -lncurses

-include $(DEPENDS)

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

lib		:
			make -C libs/sdl/
			make -C libs/ncurses/
			make -C libs/raylib/
			make -C libs/raylib/sound/

.PHONY : all clean fclean re
