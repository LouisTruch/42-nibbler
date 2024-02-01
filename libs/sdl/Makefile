CC = g++
CXXFLAGS = -Wall -Werror -Wextra -fPIC -std=c++20

LDFLAGS := -LSDL/build/build/.libs -lSDL2 -D_REENTRANT
INCLUDES = -ISDL

SRCS = $(wildcard srcs/*.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRCS))
DEPENDS := $(patsubst %.cpp, %.d, $(SRCS))
TARGET = libsdl.so

ifeq ($(DEBUG), 1)
	CXXFLAGS += -g
	CXXFLAGS += -DDEBUG
endif

ifeq ($(MANUAL_INSTALL), 1)
	CXXFLAGS += -DMANUAL_INSTALL
endif 

.PHONY: clean fclean re

yellow = /bin/echo -e "\x1B[33m$1\x1b[0m"
all:    $(TARGET)
	@$(call yellow,"Successfully compiled SDL shared object.")

-include $(DEPENDS)

$(TARGET): $(OBJS) 
	$(CXX) $(INCLUDES) -shared -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o : %.cpp Makefile
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	$(RM) $(OBJS) $(DEPENDS)

fclean: clean
	$(RM) $(TARGET)

re: fclean all